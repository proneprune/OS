savedcmd_/home/ubuntu/Desktop/OS/Seminar/chardev/chardev.mod := printf '%s\n'   chardev.o | awk '!x[$$0]++ { print("/home/ubuntu/Desktop/OS/Seminar/chardev/"$$0) }' > /home/ubuntu/Desktop/OS/Seminar/chardev/chardev.mod