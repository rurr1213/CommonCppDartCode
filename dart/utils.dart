// filepath: /C:/Dev/a22/lib/uuid_utils.dart
import 'package:uuid/uuid.dart';

String createUUIDString() {
  var uuid = Uuid();
  return uuid.v4();
}

int getUnixTime() {
  return DateTime.now().millisecondsSinceEpoch ~/ 1000;
}

