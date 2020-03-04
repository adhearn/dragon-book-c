FROM adhearn/c-dev:0.0.1

RUN apt-get install build-essentials man

RUN mkdir /app
VOLUME ["/app"]
