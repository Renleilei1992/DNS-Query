---
# Windows
if gethostbyname() failed, this solution can appoint specify DNS server to resolve the URL

## build
  - by visual studio 2015 or 2017
  
---
## purpose
  - if gethostbyname() failed, this solution can appoint specify DNS server to resolve the URL

## step
  - Initiate a DNS query request
  - Encode the URL use DNS rule
  - Construct a complete DNS request package
  - Send the DNS request package to the appointed DNS server by UDP
  - Recieve the DNS server response and then Decode the respond package
  - Get the URL resolve result

## to be continue
  - let the project can cross platform, Linux/MacOS/Windows all can use this project
