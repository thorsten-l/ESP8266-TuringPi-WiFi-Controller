FROM ubuntu:20.04

RUN apt-get update && apt-get install -y --no-install-recommends wget unzip git make \
 srecord bc xz-utils gcc python curl python3-pip python-dev build-essential \
 && pip3 install --upgrade setuptools

RUN pip3 install -U platformio
RUN pio platform install espressif8266@2.6.2 --with-package framework-arduinoespressif8266
RUN pio platform install espressif8266 --with-package framework-arduinoespressif8266

RUN /usr/local/bin/platformio update

# now this pio project is dealing with platformio 4.0 features
RUN /usr/local/bin/platformio upgrade --dev

# Prebuild one project to install the complete environment
RUN git clone https://github.com/thorsten-l/ESP8266-Arduino-Info.git
WORKDIR /ESP8266-Arduino-Info
RUN /usr/local/bin/platformio run
RUN rm -fr /ESP8266-Arduino-Info

RUN mkdir /workdir
WORKDIR /workdir

CMD ["/bin/bash"]
# CMD [ "/usr/local/bin/platformio", "run" ]
