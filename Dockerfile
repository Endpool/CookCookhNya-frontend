FROM gcc:14.2.0 AS builder

RUN apt-get update \
 && apt-get install -y cmake \
 && rm -rf /var/lib/apt/lists/*

WORKDIR /tmp
RUN wget https://github.com/conan-io/conan/releases/download/2.15.1/conan-2.15.1-linux-x86_64.tgz \
 && tar -xvf conan-2.15.1-linux-x86_64.tgz \
 && mv bin/* /bin \
 && rm conan-2.15.1-linux-x86_64.tgz

RUN conan profile detect \
 && sed -i 's/compiler.cppstd=.*/compiler.cppstd=23/' ~/.conan2/profiles/default

# pre install dependencies
RUN conan install --requires=boost/1.83.0 --build=missing

WORKDIR /deps
RUN wget https://github.com/Makcal/TgBotStater/archive/refs/tags/v0.4.2.tar.gz -O tgbotstater.tar.gz \
 && tar -xf tgbotstater.tar.gz \
 && rm tgbotstater.tar.gz \
 && cd TgBotStater-0.4.2 \
 && conan create . --build=missing

WORKDIR /app
COPY CMakeLists.txt conanfile.txt ./
RUN conan install . --build=missing

COPY . .
RUN cmake --preset=conan-release \
 && cmake --build --preset=conan-release


FROM gcc:14.2.0 AS runtime

WORKDIR /app
COPY --from=builder /app/build/Release/main .

RUN useradd -m appuser \
 && chown appuser:appuser /app/main

USER appuser

ENTRYPOINT ["./main"]
