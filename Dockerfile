FROM adhearn/c-dev:0.0.1

RUN apt-get update && apt-get install --yes --force-yes man libglib2.0-dev

RUN mkdir /app
VOLUME ["/app"]
RUN cd /app
