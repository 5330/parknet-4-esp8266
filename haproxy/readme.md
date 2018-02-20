# written for haproxy 1.7.9-1ubuntu1
#direct port 80 inbound to haproxy. 
#have dns aliases available for each sensor for your domain.
## haproxy takes the backend sever listed and serves it out via its port 80. 
## useful to add many sensors to single inbound port 80 and no front end or proxy web server accept for haproxy. 


