import 'dart:async';
import 'dart:io';

import 'package:libffi6_extension/src/installer.dart';

Future main(List<String> args) async {
  var installer = new Installer();
  var code = await installer.install(args);
  exit(code);
}
