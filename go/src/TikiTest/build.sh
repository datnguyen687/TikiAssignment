#!/bin/bash

export GOROOT=${HOME}/Documents/sdk/go;
export GOPATH=${HOME}/go;
export PATH=${PATH}:${GOROOT}/bin;

go build tikiTest.go;