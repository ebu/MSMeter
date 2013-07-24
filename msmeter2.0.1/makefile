CFLAGS = -DMETER_OS_LINUX \
         -DMETER_FILE_UNIX \
	 -D_FILE_OFFSET_BITS=64 \
	 -D_LARGEFILE_SOURCE \
	 -D_LARGEFILE64_SOURCE \
	 -D_GNU_SOURCE

meter: meter.cpp fileAccess.cpp timers.cpp meter.h network.cpp ntp.cpp
	g++ $(CFLAGS) meter.cpp timers.cpp fileAccess.cpp network.cpp ntp.cpp -o meter
