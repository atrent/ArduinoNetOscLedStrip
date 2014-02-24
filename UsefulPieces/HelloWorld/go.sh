#!/bin/bash


export CLASSPATH=$CLASSPATH:./javaoscfull.jar:./javaosc.jar

javac *java

java RemoteCommand
