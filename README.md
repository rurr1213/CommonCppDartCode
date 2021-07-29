# CommonCppDartCode
This is a submodule that is common to both the PC Matrix and Vortex Flutter app for the Ultra project. 
When you clone the PC or Flutter app with submodules - this will get cloned in the correct directory.

The contents of this directory can only be changed with care.

Only MessagesCommon.h should be changed to add new messages. 
This should be C++ and can only accept a subset of C++ keywords.
The Generate.bat windows batch file when run, will use a sed script in MessagesInc_toDart.sed to convert
the MessagesCommon.h file into a Dart version MessageCommon_generated.dart, that is then imported into the
vortex flutter project.

This is done to ensure that the Matrix and Vortex share a common code base for basic messaging.


