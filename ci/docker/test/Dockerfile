FROM melg8/denvy:0.0.2

USER root

RUN adduser -D -u 1000 test
RUN mkdir work

USER test
WORKDIR /home/user/work

HEALTHCHECK NONE