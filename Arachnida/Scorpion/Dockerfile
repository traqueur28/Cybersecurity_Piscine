FROM debian:bullseye-slim

RUN apt-get update && apt-get install -y \
    build-essential \
    libncurses5-dev \
    libncursesw5-dev \
    libreadline6-dev \
    libsqlite3-dev \
    libssl-dev \
    libxml2-dev \
    libxslt1-dev \
    libyaml-dev \
    zlib1g-dev \
    libjpeg-dev \
    libpng-dev \
    libgif-dev \
    libexiv2-dev \
    && rm -rf /var/lib/apt/lists/*

WORKDIR /app

COPY . /app

CMD ["bash"]