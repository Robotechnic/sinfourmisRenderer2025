FROM alpine:3.21

# Install required packages
RUN apk add --no-cache \
	git \
	gcc \
	g++ \
	cmake \
	make \
	sfml \
	sfml-dev \
	bash
	
SHELL ["/bin/bash", "-c"]

RUN mkdir /sinfourmis

WORKDIR /sinfourmis
