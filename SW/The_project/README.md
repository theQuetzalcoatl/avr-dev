**Versioning**: 

Right now somewhat arbitrary, but a **major.minor** is being used</br>
*major*: The change forces the user to use the SW a different way. E.g. A feature is cut, an API has changed, new build environment etc.</br>
*minor*: Changes may not directly affect the user. E.g. Bugfixes, efficiency improvement, internal restructuring etc.

**How to compile**:</br>
*  ./build.sh        (only compilation)</br>
*  ./build.sh upload (for burning as well)
___
### Aztec</br>
A lightweight, monolithic, real time embedded OS for AVR chips.
* Preemptive, using time slicing scheduling(Round Robin)
* Configurable stack size for each thread
* Stack overflow detection
* Device driver integration for the HW
* Currently 10 system calls
* Scripts for better, safer code quality
* Relatively configurable
* Mistake-proof</br>
  * stack sizes are checked for minimum & maximum
  * cannot register multiple threads using the same stack
  * min. max. system tick check, based on pysical calculations

#### Driver system</br>

##### **Related syscalls, functions**:
* _register_device_ - initializes and notifies the kernel of its existance, provides init checking upon leasing and ownership handling
* _lease_ - checks device initialization and its owner. If not owned it will be by the calling thread, if owned, an error will be raised by the kernel
* _release_ - tries to release the requested device. If the caller own it, it will be released, if not, error.
    If a thread exits without releasing any of its owned devices, the kernel releases them.
* _check_device_ownership_ - the developer of a driver can guard function calls to a device by checking the caller's ownership. It is not necessary, though.</br></br>
**Prepare a device for registration** by creating a directory under 'drivers'. Create at least an initialization function for it with type void and no arguments, and lastly, add a device ID entry to the DEVICES_IDS enum in kernel.h and include the header file.

#### Notice
More threads and longer systick period lessen the wait syscalls' accuracy.</br>
Due to required minimal stack usage the theoritical absolute maximum number of threads is about 89.</br>
Each Thread's stack usage should be estimated by the user.</br>
Threads should be exited with the proper syscall and not with a **return** statement.</br>
