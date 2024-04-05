# Options
binariesSourceDirectory=bin/Release
destinationBaseDirectory=out

# Short commit hash
shortSha=`git rev-parse --short HEAD`
# Commit count on the current HEAD
commitCount=`git rev-list HEAD --count`
# Project version from meson
projectVersion=`meson introspect --projectinfo ${binariesSourceDirectory} | jq -r '.version'`

# Destination directory
destinationDirectory="${destinationBaseDirectory}/ku-${projectVersion}.${commitCount}.${shortSha}"

# Create directory to package everything into
echo Package Destination directory: ${destinationDirectory}
mkdir -p ${destinationDirectory}

# Copy necessary files
cp ${binariesSourceDirectory}/libku.so ${destinationDirectory}
cp ${binariesSourceDirectory}/ku_unit_test ${destinationDirectory}
cp ${binariesSourceDirectory}/ku_performance_test ${destinationDirectory}
