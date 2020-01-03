# file_uploader_Downloader
A small program to upload/download files over sockets

This is a insecure file uploader/downloader that uses tcp sockets.

The program can both send and receive files.

the file sender uses bind socket to receive a connection and starts sending the file over sockets.The file send is unencrypted.

The file receiver gives a connect call to the address specified to start receiving file.


<h1>HOW TO USE</h1>
