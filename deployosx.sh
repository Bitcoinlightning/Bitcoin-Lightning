#!/bin/bash
OSX_DEPLOY_SCRIPT=contrib/macdeploy/macdeployqtplus
OSX_APP=Bitcoin_Lightning-Qt.app
OSX_QT_TRANSLATIONS=da,de,es,hu,ru,uk,zh_CN,zh_TW,fr
OSX_FANCY_PLIST=contrib/macdeploy/fancy.plist

$OSX_DEPLOY_SCRIPT $OSX_APP -add-qt-tr $OSX_QT_TRANSLATIONS -dmg -fancy $OSX_FANCY_PLIST -verbose 2


