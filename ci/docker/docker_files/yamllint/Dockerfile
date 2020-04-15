FROM python:3.7-alpine as base

FROM base as builder

RUN mkdir /install
WORKDIR /install

RUN pip install yamllint==1.21.0 --install-option="--prefix=/install" 

FROM base

COPY --from=builder /install /usr/local
CMD ["yamllint", "-"]
