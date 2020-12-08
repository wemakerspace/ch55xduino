#use
#   https://gh-proxy.deqing.workers.dev/raw.githubusercontent.com/DeqingSun/ch55xduino/playground/mirror/package_ch55xduino_mcs51_index_proxy.json

import urllib2  # the lib that handles the url stuff
target_url = "https://raw.githubusercontent.com/DeqingSun/ch55xduino/ch55xduino/package_ch55xduino_mcs51_index.json"

data = urllib2.urlopen(target_url) # it's a file like object and works just like a file


out_file = open("package_ch55xduino_mcs51_index_proxy.json", "w")
for line in data:
    line = line.replace("https://github.com","https://gh-proxy.deqing.workers.dev/github.com")
    n = out_file.write(line)
out_file.close()
