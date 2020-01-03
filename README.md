# file_uploader_Downloader
A small program to upload/download files over sockets

This is a insecure file uploader/downloader that uses tcp sockets.

The program can both send and receive files.

the file sender uses bind socket to receive a connection and starts sending the file over sockets.The file send is unencrypted.

The file receiver gives a connect call to the address specified to start receiving file.


<h1>HOW TO USE</h1>

AN UNENCRYPTED FILE UPLOADER/DOWNLOADER To receive a file simply type upload_download.exe frecv address_to_RECEIVE_from filename_to_save_as

Ex:-upload_download.exe frecv localhost test.pdf

To send a file simply type upload_download.exe fsend filename_to_send

EX:-upload_download.exe fsend test.pdf

