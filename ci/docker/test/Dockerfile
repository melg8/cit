FROM alpine:3.11.5 AS test_builder

LABEL Name=melg8/test_builder Version=0.0.0

RUN echo "i'm slow" \
 && sleep 10 \
 && ls -la \
 && echo "i'm finished"

FROM golang:1.13-alpine AS go_builder
LABEL Name=melg8/go_builder Version=0.0.3

FROM alpine:3.11.5 as test_nano
LABEL Name=test/nano Version=0.0.3
RUN ls -la

COPY --from=go_builder  /usr/local/bin/ /usr/local/bin/
COPY --from=melg8/test_builder:0.0.0 /usr /usr

RUN adduser -D -u 1000 user

USER user
WORKDIR /home/user/work

HEALTHCHECK NONE
