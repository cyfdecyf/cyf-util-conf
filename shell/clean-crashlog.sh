#!/bin/bash

# Clean crash report log on Mac OS X

rm -rf ~/Library/Logs/CrashReporter/*
rm -f ~/Library/Logs/DiagnosticReports/*
rm -f ~/Library/Application\ Support/CrashReporter/*
rm -f /Library/Logs/DiagnosticReports/*
rm -f /Library/Logs/CrashReporter/*
