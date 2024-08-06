export JAVA_HOME=/usr/lib/jvm/java-17-openjdk-amd64
export CLASSPATH=/home/developer/workspace
export LD_LIBRARY_PATH=.

cp /home/developer/images/catold.jpg /home/developer/images/cat.jpg
javac watermark/wmLib.java
jar cf wmLib.jar -C . watermark
./makejni.sh
javac -cp wmLib.jar test.java

java -cp .:wmLib.jar test
