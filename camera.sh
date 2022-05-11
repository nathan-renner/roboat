#! /bin/sh
# For use on Pi

ffmpeg -re -f s16le -i /dev/zero -f v4l2 -thread_queue_size 512 -codec:v h264 -s 1920x1080 -i /dev/video0 -codec:v copy -acodec aac -ab 128k -g 50 -f flv rtmp://a.rtmp.youtube.com/live2/m50w-pmgh-tmyk-rk1y-7w76