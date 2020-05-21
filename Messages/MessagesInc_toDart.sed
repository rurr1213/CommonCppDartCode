# This script uses windows sed and is run from the command line
#       sed -f MessagesInc_toDart.sed Messages.inc > Messages_generated.dart
1s/^/import \'package:watchman_fl\/entities\/SerDes.dart\';/
s/^};/}/g
s/:.*public/extends/g
s/public://g
s/std::string/String/g
s/short int/int/g
s/SerDes\&/SerDes/g
s/Msg::/super\./g
