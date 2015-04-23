import "dart:io";

import "package:libffi6_extension/src/installer.dart";

void main(List<String> args) {
  var cwd = Directory.current.path;
  try {
    var installer = new Installer();
    installer.install(args);
  } finally {
    Directory.current = new Directory(cwd);
  }
}
