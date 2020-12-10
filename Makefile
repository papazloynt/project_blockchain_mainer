.PHONY: all
all: install-deps generate build

.PHONY: generate
generate: install-deps .generate

.PHONY: .generate
.generate: .generate_structs .generate_service

.PHONY: .generate_structs
.generate_structs:
	mkdir -p pkg/sugest
	protoc -I protos \
		--go_out ./pkg/sugest \
		--go_opt plugins=grpc \
		--go_opt paths=source_relative \
		protos/sugest.proto

.PHONY: .generate_service
.generate_service:
	mkdir -p pkg/sugest
	protoc -I protos --grpc-gateway_out ./pkg/sugest \
			 --grpc-gateway_opt=logtostderr=true \
			 --grpc-gateway_opt=paths=source_relative \
			 --grpc-gateway_opt=generate_unbound_methods=true \
		protos/sugestgi.proto

.PHONY: build
build: generate .build

.PHONY: .build
.build: build-gateway build-grpc-service

.PHONY: build-gateway
build-gateway: generate install-golang .build-gateway

.PHONY: .build-gateway
.build-gateway:
	go build -o gateway gateway.go

.PHONY: build-grpc-service
build-grpc-service:
	./tools/polly/bin/polly

.PHONY: run
run: run-grpc-service

.PHONY: run-gateway
run-gateway: build-gateway .run-gateway

.PHONY: .run-gateway
.run-gateway:
	./gateway

.PHONY: run-grpc-service
run-grpc-service: build-grpc-service .run-grpc-service

.PHONY: .run-grpc-service
.run-grpc-service:
	./_builds/default/grpc_service

.PHONY: init-submodule
init-submodule: intall-git .init-submodule

.PHONY: .init-submodule
.init-submodule:
	git submodule update --init

.PHONY: install-deps
install-deps: install-golang install-protobuf install-go-deps install-cpp-deps

.PHONY: install-golang
install-golang: install-brew .install-golang

.PHONY: .install-golang
.install-golang:
	which go || brew install golang

.PHONY: install-protobuf
install-protobuf: install-brew .install-protobuf

.PHONY: .install-protobuf
.install-protobuf:
	which protoc || brew install protobuf

.PHONY: install-go-deps
install-go-deps: install-golang .install-go-deps

.PHONY: .install-go-deps
.install-go-deps:
	ls go.mod || go mod init goelster
	go get -u github.com/grpc-ecosystem/grpc-gateway/protoc-gen-grpc-gateway
	go get -u github.com/golang/protobuf/proto
	go get -u github.com/golang/protobuf/protoc-gen-go
	go install google.golang.org/grpc/cmd/protoc-gen-go-grpc

.PHONY: install-cpp-deps
install-cpp-deps: install-git install-cmake .install-cpp-deps

.PHONY: install-brew
install-brew:
	which brew || /bin/bash -c "$$(curl -fsSL https://raw.githubusercontent.com/Homebrew/install/master/install.sh)"

.PHONY: install-git
install-git: install-brew .install-git

.PHONY: .install-git
.install-git:
	which git || brew install git

.PHONY: install-cmake
install-cmake: install-brew .install-cmake

.PHONY: .install-cmake
.install-cmake:
	which cmake || brew install cmake

GRPC_CPP_PLUGIN_EXISTS := $(shell PATH=$$PATH:$$HOME/bin which grpc_cpp_plugin 2> /dev/null)

.PHONY: .install-cpp-deps
.install-cpp-deps:
ifndef GRPC_CPP_PLUGIN_EXISTS
	rm -rf grpc && \
	git clone https://github.com/grpc/grpc --recursive && \
	cd grpc && \
	mkdir -p cmake/build && \
	cd cmake/build && \
	cmake ../.. && \
	make && \
	mkdir -p $(HOME)/bin && \
	cp grpc_cpp_plugin $(HOME)/bin && \
	cd ../../.. && \
	rm -rf grpc
endif

.PHONY: config
config: install-sed .config

.PHONY: install-sed
install-sed:
	which sed || brew install sed

GO_PACKAGE_PATH:=$(shell pwd | sed -e "s,.*go\/src/github.com/\(.*\),\1,")
.PHONY: .config
.config:
	sed -i 's,bmstu-iu8-cpp-sem-3/lab-07-grpc,$(GO_PACKAGE_PATH),' gateway.go
