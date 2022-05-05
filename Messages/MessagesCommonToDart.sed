# This script uses windows sed and is run from the command line
#       sed -f MessagesInc_toDart.sed Messages.inc > Messages_generated.dart
1s/^/import \'..\/..\/SerDes.dart\';/
s/^};/}/g
s/:.*public/extends/g
s/public://g
s/short int/int/g
s/std::string/String/g
s/std::list/List/g
s/RSerDes/SerDes/g
s/virtual //g
s/int64_t/int/g
s/statList;/statList = [];/g
s/inline//g
s/nlohmann::json/Map<String, Dynamic>/g
s/json::parse/jsonDecode/g
s/void//g
#
# The following take a word of the form
# M_XXX(parm1,parm2) where each parm is identfied by
# being between \( and \). The first is \1 the second is \2
s/M_ALLOCATELIST(\(.*\),\(.*\))/\2 = List<\1>();/g
s/M_DECLAREVARIABLE(\(.*\),\(.*\))/var \2 = \1()/g
s/M_BASECLASS(\([-a-zA-Z0-9_]*\),\(.*\))/super.\2/g
s/M_LISTFORLOOPSTART(\(.*\),\(.*\))/for ( var \1 in \2 ) {/g
s/M_LISTLEN(\([-a-zA-Z0-9_]*\))/\1.length/g
s/M_CPPONLY(\(.*\))//g
s/M_SIZE(\([-a-zA-Z0-9_]*\))/\1.length/g
s/M_ELEMENT(\([-a-zA-Z0-9_]*\),\(.*\))/\1.codeUnitAt(\2)/g
s/M_JSONPAIR(\([-a-zA-Z0-9_]*\),\(.*\))/\1:\2,/g
s/M_ENUM(\([-a-zA-Z0-9_]*\),\(.*\))/\1.\2,/g
