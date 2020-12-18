# Core Flight System : Framework : Tool : Table CRC Generator

This repository contains NASA's Table CRC Generator Tool (tblCRCTool), which is a framework component of the Core Flight System.

This lab application is a ground utility to generate binary table CRCs for cFS. It is intended to be located in the `tools/tblCRCTool` subdirectory of a cFS Mission Tree.  The Core Flight System is bundled at https://github.com/nasa/cFS (which includes this tool as a submodule), which includes build and execution instructions.

## Version Notes

### Development Build: 1.2.0-rc1+dev3

- Use `sizeof()` instead of a hard coded value for the table file header size to keep this tool in sync if the size of the cFE file or table header should ever change.
- Update version baseline to v1.2.0-rc1
- Set REVISION number to 99 to indicate development version
See <https://github.com/nasa/tblCRCTool/pull/25>

### Development Build: 1.1.0+dev7

- Create a version header file
- Report version when responding to `-help` command
- See <https://github.com/nasa/tblCRCTool/pull/22>

### Development Build: 1.1.1

- Apply Code Style
- See <https://github.com/nasa/tblCRCTool/pull/18>

### **_OFFICIAL RELEASE: 1.1.0 - Aquila_**

- Minor updates (see https://github.com/nasa/tblCRCTool/pull/12)
- Released as part of cFE 6.7.0, Apache 2.0

### **_OFFICIAL RELEASE: 1.0.0a_**

- Released as part of cFE 6.6.0a, Apache 2.0

NOTE - there are other parameter set management schemes used with the cFS (JSON, csv, etc) which may be more applicable for modern missions.  Contact the community as detailed below for more information.

## Known issues

This ground utility was developed for a specific mission/configuration, and may not be applicable for general use.

## Getting Help

For best results, submit issues:questions or issues:help wanted requests at https://github.com/nasa/cFS.

Official cFS page: http://cfs.gsfc.nasa.gov
