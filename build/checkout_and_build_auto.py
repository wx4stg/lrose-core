#!/usr/bin/env python

#===========================================================================
#
# Build LROSE release, using autoconf and configure
#
#===========================================================================

from __future__ import print_function
import os
import sys
import shutil
import subprocess
from optparse import OptionParser
import time
from datetime import datetime
from datetime import date
from datetime import timedelta
import glob
from sys import platform

def main():

    # globals

    global thisScriptName
    thisScriptName = os.path.basename(__file__)

    global options
    global package
    global prefix
    global releaseName
    global netcdfDir
    global displaysDir
    global coreDir
    global codebaseDir
    global codebasePath
    global scratchBuildDir
    global tmpBinDir
    global binDir
    global libDir
    global includeDir
    global shareDir
    global versionStr
    global debugStr
    global logPath

    # parse the command line

    usage = "usage: %prog [options]"
    homeDir = os.environ['HOME']
    prefixDirDefault = os.path.join(homeDir, 'lrose')
    buildDirDefault = '/tmp/lrose_build'
    logDirDefault = '/tmp/lrose_build/logs'
    parser = OptionParser(usage)
    parser.add_option('--clean',
                      dest='clean', default=False,
                      action="store_true",
                      help='Cleanup tmp build dir')
    parser.add_option('--debug',
                      dest='debug', default=True,
                      action="store_true",
                      help='Set debugging on')
    parser.add_option('--verbose',
                      dest='verbose', default=False,
                      action="store_true",
                      help='Set verbose debugging on')
    parser.add_option('--package',
                      dest='package', default='lrose-core',
                      help='Package name. Options are: ' + \
                      'lrose-core (default), cidd, radx, titan, lrose-blaze')
    parser.add_option('--lroseCoreTag',
                      dest='lroseCoreTag', default='master',
                      help='Tag to check out lrose-core')
    parser.add_option('--prefix',
                      dest='prefix', default=prefixDirDefault,
                      help='Install directory')
    parser.add_option('--buildDir',
                      dest='buildDir', default=buildDirDefault,
                      help='Temporary build dir')
    parser.add_option('--logDir',
                      dest='logDir', default=logDirDefault,
                      help='Logging dir')
    parser.add_option('--static',
                      dest='static', default=False,
                      action="store_true",
                      help='use static linking, default is dynamic')
    parser.add_option('--scripts',
                      dest='installScripts', default=False,
                      action="store_true",
                      help='Install scripts as well as binaries')
    parser.add_option('--useSystemNetcdf',
                      dest='useSystemNetcdf', default=False,
                      action="store_true",
                      help='Use system install of NetCDF and HDF5 instead of building it here')

    (options, args) = parser.parse_args()
    
    if (options.verbose):
        options.debug = True

    # for CIDD, set to static linkage
    if (options.package == "cidd"):
        options.static = True
        
    debugStr = " "
    if (options.verbose):
        debugStr = " --verbose "
    elif (options.debug):
        debugStr = " --debug "

    package = options.package
    prefix = options.prefix

    # runtime

    now = time.gmtime()
    nowTime = datetime(now.tm_year, now.tm_mon, now.tm_mday,
                       now.tm_hour, now.tm_min, now.tm_sec)
    versionStr = nowTime.strftime("%Y%m%d")

    # set directories
    
    scratchBuildDir = os.path.join(options.buildDir, 'scratch')
    coreDir = os.path.join(options.buildDir, "lrose-core")
    displaysDir = os.path.join(options.buildDir, "lrose-displays")
    netcdfDir = os.path.join(options.buildDir, "lrose-netcdf")
    codebaseDir = os.path.join(coreDir, "codebase")
    releaseName = options.package + "-" + versionStr + ".src"
    
    tmpBinDir = os.path.join(scratchBuildDir, 'bin')
    binDir = os.path.join(prefix, 'bin')
    libDir = os.path.join(prefix, 'lib')
    includeDir = os.path.join(prefix, 'include')
    shareDir = os.path.join(prefix, 'share')
    
    logPath = os.path.join(options.logDir, "master");

    if (options.debug):
        print("Running %s:" % thisScriptName, file=sys.stderr)
        print("  lroseCoreTag: ", options.lroseCoreTag, file=sys.stderr)
        print("  package: ", package, file=sys.stderr)
        print("  releaseName: ", releaseName, file=sys.stderr)
        print("  static: ", options.static, file=sys.stderr)
        print("  buildDir: ", options.buildDir, file=sys.stderr)
        print("  logDir: ", options.logDir, file=sys.stderr)
        print("  coreDir: ", coreDir, file=sys.stderr)
        print("  codebaseDir: ", codebaseDir, file=sys.stderr)
        print("  displaysDir: ", displaysDir, file=sys.stderr)
        print("  netcdfDir: ", netcdfDir, file=sys.stderr)
        print("  prefixDir: ", prefix, file=sys.stderr)
        print("  binDir: ", binDir, file=sys.stderr)
        print("  libDir: ", libDir, file=sys.stderr)
        print("  includeDir: ", includeDir, file=sys.stderr)
        print("  shareDir: ", shareDir, file=sys.stderr)
        print("  useSystemNetcdf: ", options.useSystemNetcdf, file=sys.stderr)

    # create build dir
    
    createBuildDir()

    # make tmp dirs

    try:
        os.makedirs(scratchBuildDir)
        os.makedirs(tmpBinDir)
        os.makedirs(options.logDir)
    except:
        print("  note - dirs already exist", file=sys.stderr)

    # get repos from git

    logPath = prepareLogFile("git-checkout");
    gitCheckout()

    # install the distribution-specific makefiles

    logPath = prepareLogFile("install-package-makefiles");
    os.chdir(codebaseDir)
    shellCmd("./make_bin/install_package_makefiles.py --package " + 
             package + " --codedir .")

    # trim libs and apps to those required by distribution makefiles

    if (package != "lrose"):
        trimToMakefiles("libs")
        trimToMakefiles("apps")

    # set up autoconf

    logPath = prepareLogFile("setup-autoconf");
    setupAutoconf()

    # create the release information file
    
    createReleaseInfoFile()

    # run qmake for QT apps to create moc_ files

    logPath = prepareLogFile("create-qt-moc-files");
    hawkEyeDir = os.path.join(codebaseDir, "apps/radar/src/HawkEye")
    createQtMocFiles(hawkEyeDir)

    # prune any empty directories

    prune(codebaseDir)

    # build netcdf support
    
    if (options.useSystemNetcdf == False):
        logPath = prepareLogFile("build-netcdf");
        buildNetcdf()

    # build the package

    buildPackage()

    # detect which dynamic libs are needed
    # copy the dynamic libraries into a directory relative
    # to the binary install dir:
    #     bin/${package}_runtime_libs

    if (platform != "darwin"):
        os.chdir(codebaseDir)
        libRelDir = package + "_runtime_libs"
        cmd = "./make_bin/installOriginLibFiles.py" + \
              " --binDir " + tmpBinDir + \
              " --relDir " + libRelDir
        if (options.verbose):
            cmd = cmd + " --verbose"
        elif (options.debug):
            cmd = cmd + " --debug"
        shellCmd(cmd)

    # perform the install

    logPath = prepareLogFile("do-final-install");
    doFinalInstall();

    # check the install

    logPath = prepareLogFile("no-logging");
    checkInstall()
    
    # delete the tmp dir

    if (options.clean):
        shutil.rmtree(options.buildDir)

    sys.exit(0)

########################################################################
# create the build dir

def createBuildDir():

    # check if exists already

    if (os.path.isdir(options.buildDir)):

        print(("WARNING: you are about to remove all contents in dir: " + 
              options.buildDir))
        print("===============================================")
        contents = os.listdir(options.buildDir)
        for filename in contents:
            print(("  " + filename))
        print("===============================================")
        answer = "n"
        if (sys.version_info > (3, 0)):
            answer = input("WARNING: do you wish to proceed (y/n)? ")
        else:
            answer = raw_input("WARNING: do you wish to proceed (y/n)? ")
        if (answer != "y"):
            print("  aborting ....")
            sys.exit(1)
                
        # remove it

        shutil.rmtree(options.buildDir)

    # make it clean
    
    print(("INFO: you are about to create build dir: " + 
          options.buildDir))
    
    os.makedirs(options.buildDir)

########################################################################
# check out repos from git

def gitCheckout():

    os.chdir(options.buildDir)

    # lrose core

    shellCmd("git clone --branch " + options.lroseCoreTag + \
             " https://github.com/NCAR/lrose-core")

    # netcdf and hdf5

    if (options.useSystemNetcdf == False):
        shellCmd("git clone https://github.com/NCAR/lrose-netcdf")

    # color scales and maps in displays repo

    shellCmd("git clone https://github.com/NCAR/lrose-displays")

########################################################################
# set up autoconf for configure etc

def setupAutoconf():

    os.chdir(codebaseDir)

    # create files for configure

    shutil.copy("../build/Makefile.top", "Makefile")

    if (options.static):
        if (package == "cidd"):
             shutil.copy("../build/autoconf/configure.base.cidd", "./configure.base")
        else:
             shutil.copy("../build/autoconf/configure.base", "./configure.base")
        shellCmd("./make_bin/createConfigure.am.py --dir ." +
                 " --baseName configure.base" +
                 " --pkg " + package + debugStr)
    else:
        if (package == "cidd"):
            shutil.copy("../build/autoconf/configure.base.shared.cidd",
                        "./configure.base.shared")
        else:
            shutil.copy("../build/autoconf/configure.base.shared",
                        "./configure.base.shared")
        shellCmd("./make_bin/createConfigure.am.py --dir ." +
                 " --baseName configure.base.shared --shared" +
                 " --pkg " + package + debugStr)

########################################################################
# Run qmake for QT apps such as HawkEye to create _moc files

def createQtMocFiles(appDir):
    
    if (os.path.isdir(appDir) == False):
        return

    os.chdir(appDir)
    shellCmd("rm -f moc*");
    shellCmd("qmake-qt5 -o Makefile.qmake");
    shellCmd("make -f Makefile.qmake mocables");

########################################################################
# write release information file

def createReleaseInfoFile():

    # go to core dir

    os.chdir(scratchBuildDir)

    # open info file

    releaseInfoPath = os.path.join(coreDir, "ReleaseInfo.txt")
    info = open(releaseInfoPath, 'w')

    # write release info

    info.write("package:" + package + "\n")
    info.write("version:" + versionStr + "\n")
    info.write("release:" + releaseName + "\n")

    # close

    info.close()

########################################################################
# get string value based on search key
# the string may span multiple lines
#
# Example of keys: SRCS, SUB_DIRS, MODULE_NAME, TARGET_FILE
#
# value is returned

def getValueListForKey(path, key):

    valueList = []

    try:
        fp = open(path, 'r')
    except IOError as e:
        if (options.verbose):
            print("ERROR - ", thisScriptName, file=sys.stderr)
            print("  Cannot open file:", path, file=sys.stderr)
        return valueList

    lines = fp.readlines()
    fp.close()

    foundKey = False
    multiLine = ""
    for line in lines:
        if (foundKey == False):
            if (line[0] == '#'):
                continue
        if (line.find(key) >= 0):
            foundKey = True
            multiLine = multiLine + line
            if (line.find("\\") < 0):
                break;
        elif (foundKey):
            if (line[0] == '#'):
                break
            if (len(line) < 2):
                break
            multiLine = multiLine + line;
            if (line.find("\\") < 0):
                break;

    if (foundKey == False):
        return valueList

    multiLine = multiLine.replace(key, " ")
    multiLine = multiLine.replace("=", " ")
    multiLine = multiLine.replace("\t", " ")
    multiLine = multiLine.replace("\\", " ")
    multiLine = multiLine.replace("\r", " ")
    multiLine = multiLine.replace("\n", " ")

    toks = multiLine.split(' ')
    for tok in toks:
        if (len(tok) > 0):
            valueList.append(tok)

    return valueList

########################################################################
# Trim libs and apps to those required by distribution

def trimToMakefiles(subDir):

    if (options.verbose):
        print("Trimming unneeded dirs, subDir: " + subDir, file=sys.stderr)

    # get list of subdirs in makefile

    dirPath = os.path.join(codebaseDir, subDir)
    os.chdir(dirPath)

    # need to allow upper and lower case Makefile (makefile or Makefile)
    subNameList = getValueListForKey("makefile", "SUB_DIRS")
    if not subNameList:
        if (options.verbose):
            print("Trying uppercase Makefile ... ", file=sys.stderr)
        subNameList = getValueListForKey("Makefile", "SUB_DIRS")
    
    for subName in subNameList:
        if (os.path.isdir(subName)):
            if (options.verbose):
                print("  need sub dir: " + subName, file=sys.stderr)
            
    # get list of files in subDir

    entries = os.listdir(dirPath)
    for entry in entries:
        theName = os.path.join(dirPath, entry)
        if (options.verbose):
            print("considering: " + theName, file=sys.stderr)
        if (entry == "scripts") or (entry == "include"):
            # always keep scripts directories
            continue
        if (os.path.isdir(theName)):
            if (entry not in subNameList):
                if (options.verbose):
                    print("discarding it", file=sys.stderr)
                shutil.rmtree(theName)
            else:
                if (options.verbose):
                    print("keeping it and recurring", file=sys.stderr)
                # check this child's required subdirectories ( recurse )
                # nextLevel = os.path.join(dirPath, entry)
                # print >> sys.stderr, "trim to makefile on subdirectory: "
                trimToMakefiles(os.path.join(subDir, entry))

########################################################################
# build netCDF

def buildNetcdf():

    os.chdir(netcdfDir)
    if (package == "cidd"):
        shellCmd("./build_and_install_netcdf.m32 -x " + scratchBuildDir)
    else:
        if platform == "darwin":
            shellCmd("./build_and_install_netcdf.osx -x " + scratchBuildDir)
        else:
            shellCmd("./build_and_install_netcdf -x " + scratchBuildDir)

########################################################################
# build package

def buildPackage():

    global logPath

    # set the environment

    os.environ["LDFLAGS"] = "-L" + scratchBuildDir + "/lib " + \
                            " -Wl,-rpath,'$$ORIGIN/" + package + "_runtime_libs:" + \
                            scratchBuildDir + "/lib'"
    os.environ["FC"] = "gfortran"
    os.environ["F77"] = "gfortran"
    os.environ["F90"] = "gfortran"

    if (platform == "darwin"):
        os.environ["PKG_CONFIG_PATH"] = "/usr/local/opt/qt/lib/pkgconfig"
    else:
        os.environ["CXXFLAGS"] = " -std=c++11 "

    # print out environment

    logPath = prepareLogFile("print-environment");
    cmd = "env"
    shellCmd(cmd)

    # run configure

    logPath = prepareLogFile("run-configure");
    os.chdir(codebaseDir)
    if (options.useSystemNetcdf):
        cmd = "./configure --prefix=" + scratchBuildDir
    else:
        cmd = "./configure --with-hdf5=" + scratchBuildDir + \
              " --with-netcdf=" + scratchBuildDir + \
                                " --prefix=" + scratchBuildDir
    shellCmd(cmd)

    # build the libraries

    logPath = prepareLogFile("build-libs");
    os.chdir(os.path.join(codebaseDir, "libs"))
    cmd = "make -k -j 8"
    shellCmd(cmd)

    # install the libraries

    logPath = prepareLogFile("install-libs-to-tmp");

    cmd = "make -k install-strip"
    shellCmd(cmd)

    # build the apps

    logPath = prepareLogFile("build-apps");
    os.chdir(os.path.join(codebaseDir, "apps"))
    cmd = "make -k -j 8"
    shellCmd(cmd)

    # install the apps

    logPath = prepareLogFile("install-apps-to-tmp");
    cmd = "make -k install-strip"
    shellCmd(cmd)

    # optionally install the scripts

    if (options.installScripts):

        logPath = prepareLogFile("install-scripts-to-tmp");

        # install perl5
        
        perl5Dir = os.path.join(prefix, "lib/perl5")
        try:
            os.makedirs(perl5Dir)
        except:
            print("Dir exists: " + perl5Dir, file=sys.stderr)

        perl5LibDir = os.path.join(codebaseDir, "libs/perl5/src")
        if (os.path.isdir(perl5LibDir)):
            os.chdir(os.path.join(codebaseDir, "libs/perl5/src"))
            shellCmd("rsync -av *pm " + perl5Dir)

        # procmap

        procmapScriptsDir = os.path.join(codebaseDir, "apps/procmap/src/scripts")
        if (os.path.isdir(procmapScriptsDir)):
            os.chdir(procmapScriptsDir)
            shellCmd("./install_scripts.lrose " + binDir)

        # general

        generalScriptsDir = os.path.join(codebaseDir, "apps/scripts/src")
        if (os.path.isdir(generalScriptsDir)):
            os.chdir(generalScriptsDir)
            shellCmd("./install_scripts.lrose " + binDir)

########################################################################
# perform final install

def doFinalInstall():

    # make target dirs

    try:
        os.makedirs(binDir)
        os.makedirs(libDir)
        os.makedirs(includeDir)
        os.makedirs(shareDir)
    except:
        print("  note - dirs already exist", file=sys.stderr)
    
    # install docs etc
    
    os.chdir(coreDir)

    shellCmd("rsync -av LICENSE.txt " + prefix)
    shellCmd("rsync -av release_notes " + prefix)
    shellCmd("rsync -av docs " + prefix)

    if (package == "cidd"):
        shellCmd("rsync -av ./codebase/apps/cidd/src/CIDD/example_scripts " +
                 options.prefix)

    # install color scales

    os.chdir(displaysDir)
    shellCmd("rsync -av color_scales " + shareDir)

    # install binaries and libs

    os.chdir(scratchBuildDir)

    shellCmd("rsync -av bin " + prefix)
    shellCmd("rsync -av lib " + prefix)
    shellCmd("rsync -av include " + prefix)

########################################################################
# check the install

def checkInstall():

    os.chdir(coreDir)
    print(("============= Checking libs for " + package + " ============="))
    shellCmd("./codebase/make_bin/check_libs.py " + \
             "--listPath ./build/checklists/libs_check_list." + package + " " + \
             "--libDir " + prefix + "/lib " + \
             "--label " + package + " --maxAge 3600")
    print("====================================================")
    print(("============= Checking apps for " + package + " ============="))
    shellCmd("./codebase/make_bin/check_apps.py " + \
             "--listPath ./build/checklists/apps_check_list." + package + " " + \
             "--appDir " + prefix + "/bin " + \
             "--label " + package + " --maxAge 3600")
    print("====================================================")
    
    print("**************************************************")
    print("*** Done building auto release *******************")
    print(("*** Installed in dir: " + prefix + " ***"))
    print("**************************************************")

########################################################################
# prune empty dirs

def prune(tree):

    # walk the tree
    if (os.path.isdir(tree)):
        contents = os.listdir(tree)

        if (len(contents) == 0):
            if (options.verbose):
                print("pruning empty dir: " + tree, file=sys.stderr)
            shutil.rmtree(tree)
        else:
            for l in contents:
                # remove CVS directories
                if (l == "CVS") or (l == ".git"): 
                    thepath = os.path.join(tree,l)
                    if (options.verbose):
                        print("pruning dir: " + thepath, file=sys.stderr)
                    shutil.rmtree(thepath)
                else:
                    thepath = os.path.join(tree,l)
                    if (os.path.isdir(thepath)):
                        prune(thepath)
            # check if this tree is now empty
            newcontents = os.listdir(tree)
            if (len(newcontents) == 0):
                if (options.verbose):
                    print("pruning empty dir: " + tree, file=sys.stderr)
                shutil.rmtree(tree)

########################################################################
# prepare log file

def prepareLogFile(logFileName):

    logPath = os.path.join(options.logDir, logFileName + ".log");
    if (logPath.find('no-logging') >= 0):
        return logPath
    print("========================= " + logFileName + " =========================", file=sys.stderr)
    if (options.verbose):
        print("====>> Creating log file: " + logPath + " <<==", file=sys.stderr)
    fp = open(logPath, "w+")
    fp.write("===========================================\n")
    fp.write("Log file from script: " + thisScriptName + "\n")
    fp.close()

    return logPath

########################################################################
# Run a command in a shell, wait for it to complete

def shellCmd(cmd):

    print("Running cmd:", cmd, file=sys.stderr)
    
    if (logPath.find('no-logging') >= 0):
        cmdToRun = cmd
    else:
        print("Log file is:", logPath, file=sys.stderr)
        print("    ....", file=sys.stderr)
        cmdToRun = cmd + " 1>> " + logPath + " 2>&1"

    try:
        retcode = subprocess.check_call(cmdToRun, shell=True)
        if retcode != 0:
            print("Child exited with code: ", retcode, file=sys.stderr)
            sys.exit(1)
        else:
            if (options.verbose):
                print("Child returned code: ", retcode, file=sys.stderr)
    except OSError as e:
        print("Execution failed:", e, file=sys.stderr)
        sys.exit(1)

    print("    done", file=sys.stderr)
    
########################################################################
# Run - entry point

if __name__ == "__main__":
   main()
