// Generated by the gRPC C++ plugin.
// If you make any local change, they will be lost.
// source: blockchain.proto

#include "blockchain.pb.h"
#include "blockchain.grpc.pb.h"

#include <functional>
#include <grpcpp/impl/codegen/async_stream.h>
#include <grpcpp/impl/codegen/async_unary_call.h>
#include <grpcpp/impl/codegen/channel_interface.h>
#include <grpcpp/impl/codegen/client_unary_call.h>
#include <grpcpp/impl/codegen/client_callback.h>
#include <grpcpp/impl/codegen/message_allocator.h>
#include <grpcpp/impl/codegen/method_handler.h>
#include <grpcpp/impl/codegen/rpc_service_method.h>
#include <grpcpp/impl/codegen/server_callback.h>
#include <grpcpp/impl/codegen/server_callback_handlers.h>
#include <grpcpp/impl/codegen/server_context.h>
#include <grpcpp/impl/codegen/service_type.h>
#include <grpcpp/impl/codegen/sync_stream.h>
namespace blockchain {

static const char* Blockchain_method_names[] = {
  "/blockchain.Blockchain/Transaction",
  "/blockchain.Blockchain/Registration",
  "/blockchain.Blockchain/Authorization",
  "/blockchain.Blockchain/AddMoney",
  "/blockchain.Blockchain/InfoBalance",
};

std::unique_ptr< Blockchain::Stub> Blockchain::NewStub(const std::shared_ptr< ::grpc::ChannelInterface>& channel, const ::grpc::StubOptions& options) {
  (void)options;
  std::unique_ptr< Blockchain::Stub> stub(new Blockchain::Stub(channel));
  return stub;
}

Blockchain::Stub::Stub(const std::shared_ptr< ::grpc::ChannelInterface>& channel)
  : channel_(channel), rpcmethod_Transaction_(Blockchain_method_names[0], ::grpc::internal::RpcMethod::NORMAL_RPC, channel)
  , rpcmethod_Registration_(Blockchain_method_names[1], ::grpc::internal::RpcMethod::NORMAL_RPC, channel)
  , rpcmethod_Authorization_(Blockchain_method_names[2], ::grpc::internal::RpcMethod::NORMAL_RPC, channel)
  , rpcmethod_AddMoney_(Blockchain_method_names[3], ::grpc::internal::RpcMethod::NORMAL_RPC, channel)
  , rpcmethod_InfoBalance_(Blockchain_method_names[4], ::grpc::internal::RpcMethod::NORMAL_RPC, channel)
  {}

::grpc::Status Blockchain::Stub::Transaction(::grpc::ClientContext* context, const ::blockchain::TransactionRequest& request, ::blockchain::TransactionResponse* response) {
  return ::grpc::internal::BlockingUnaryCall(channel_.get(), rpcmethod_Transaction_, context, request, response);
}

void Blockchain::Stub::experimental_async::Transaction(::grpc::ClientContext* context, const ::blockchain::TransactionRequest* request, ::blockchain::TransactionResponse* response, std::function<void(::grpc::Status)> f) {
  ::grpc_impl::internal::CallbackUnaryCall(stub_->channel_.get(), stub_->rpcmethod_Transaction_, context, request, response, std::move(f));
}

void Blockchain::Stub::experimental_async::Transaction(::grpc::ClientContext* context, const ::grpc::ByteBuffer* request, ::blockchain::TransactionResponse* response, std::function<void(::grpc::Status)> f) {
  ::grpc_impl::internal::CallbackUnaryCall(stub_->channel_.get(), stub_->rpcmethod_Transaction_, context, request, response, std::move(f));
}

void Blockchain::Stub::experimental_async::Transaction(::grpc::ClientContext* context, const ::blockchain::TransactionRequest* request, ::blockchain::TransactionResponse* response, ::grpc::experimental::ClientUnaryReactor* reactor) {
  ::grpc_impl::internal::ClientCallbackUnaryFactory::Create(stub_->channel_.get(), stub_->rpcmethod_Transaction_, context, request, response, reactor);
}

void Blockchain::Stub::experimental_async::Transaction(::grpc::ClientContext* context, const ::grpc::ByteBuffer* request, ::blockchain::TransactionResponse* response, ::grpc::experimental::ClientUnaryReactor* reactor) {
  ::grpc_impl::internal::ClientCallbackUnaryFactory::Create(stub_->channel_.get(), stub_->rpcmethod_Transaction_, context, request, response, reactor);
}

::grpc::ClientAsyncResponseReader< ::blockchain::TransactionResponse>* Blockchain::Stub::AsyncTransactionRaw(::grpc::ClientContext* context, const ::blockchain::TransactionRequest& request, ::grpc::CompletionQueue* cq) {
  return ::grpc_impl::internal::ClientAsyncResponseReaderFactory< ::blockchain::TransactionResponse>::Create(channel_.get(), cq, rpcmethod_Transaction_, context, request, true);
}

::grpc::ClientAsyncResponseReader< ::blockchain::TransactionResponse>* Blockchain::Stub::PrepareAsyncTransactionRaw(::grpc::ClientContext* context, const ::blockchain::TransactionRequest& request, ::grpc::CompletionQueue* cq) {
  return ::grpc_impl::internal::ClientAsyncResponseReaderFactory< ::blockchain::TransactionResponse>::Create(channel_.get(), cq, rpcmethod_Transaction_, context, request, false);
}

::grpc::Status Blockchain::Stub::Registration(::grpc::ClientContext* context, const ::blockchain::RegistrationRequest& request, ::blockchain::RegistrationResponse* response) {
  return ::grpc::internal::BlockingUnaryCall(channel_.get(), rpcmethod_Registration_, context, request, response);
}

void Blockchain::Stub::experimental_async::Registration(::grpc::ClientContext* context, const ::blockchain::RegistrationRequest* request, ::blockchain::RegistrationResponse* response, std::function<void(::grpc::Status)> f) {
  ::grpc_impl::internal::CallbackUnaryCall(stub_->channel_.get(), stub_->rpcmethod_Registration_, context, request, response, std::move(f));
}

void Blockchain::Stub::experimental_async::Registration(::grpc::ClientContext* context, const ::grpc::ByteBuffer* request, ::blockchain::RegistrationResponse* response, std::function<void(::grpc::Status)> f) {
  ::grpc_impl::internal::CallbackUnaryCall(stub_->channel_.get(), stub_->rpcmethod_Registration_, context, request, response, std::move(f));
}

void Blockchain::Stub::experimental_async::Registration(::grpc::ClientContext* context, const ::blockchain::RegistrationRequest* request, ::blockchain::RegistrationResponse* response, ::grpc::experimental::ClientUnaryReactor* reactor) {
  ::grpc_impl::internal::ClientCallbackUnaryFactory::Create(stub_->channel_.get(), stub_->rpcmethod_Registration_, context, request, response, reactor);
}

void Blockchain::Stub::experimental_async::Registration(::grpc::ClientContext* context, const ::grpc::ByteBuffer* request, ::blockchain::RegistrationResponse* response, ::grpc::experimental::ClientUnaryReactor* reactor) {
  ::grpc_impl::internal::ClientCallbackUnaryFactory::Create(stub_->channel_.get(), stub_->rpcmethod_Registration_, context, request, response, reactor);
}

::grpc::ClientAsyncResponseReader< ::blockchain::RegistrationResponse>* Blockchain::Stub::AsyncRegistrationRaw(::grpc::ClientContext* context, const ::blockchain::RegistrationRequest& request, ::grpc::CompletionQueue* cq) {
  return ::grpc_impl::internal::ClientAsyncResponseReaderFactory< ::blockchain::RegistrationResponse>::Create(channel_.get(), cq, rpcmethod_Registration_, context, request, true);
}

::grpc::ClientAsyncResponseReader< ::blockchain::RegistrationResponse>* Blockchain::Stub::PrepareAsyncRegistrationRaw(::grpc::ClientContext* context, const ::blockchain::RegistrationRequest& request, ::grpc::CompletionQueue* cq) {
  return ::grpc_impl::internal::ClientAsyncResponseReaderFactory< ::blockchain::RegistrationResponse>::Create(channel_.get(), cq, rpcmethod_Registration_, context, request, false);
}

::grpc::Status Blockchain::Stub::Authorization(::grpc::ClientContext* context, const ::blockchain::AuthorizationRequest& request, ::blockchain::AuthorizationResponse* response) {
  return ::grpc::internal::BlockingUnaryCall(channel_.get(), rpcmethod_Authorization_, context, request, response);
}

void Blockchain::Stub::experimental_async::Authorization(::grpc::ClientContext* context, const ::blockchain::AuthorizationRequest* request, ::blockchain::AuthorizationResponse* response, std::function<void(::grpc::Status)> f) {
  ::grpc_impl::internal::CallbackUnaryCall(stub_->channel_.get(), stub_->rpcmethod_Authorization_, context, request, response, std::move(f));
}

void Blockchain::Stub::experimental_async::Authorization(::grpc::ClientContext* context, const ::grpc::ByteBuffer* request, ::blockchain::AuthorizationResponse* response, std::function<void(::grpc::Status)> f) {
  ::grpc_impl::internal::CallbackUnaryCall(stub_->channel_.get(), stub_->rpcmethod_Authorization_, context, request, response, std::move(f));
}

void Blockchain::Stub::experimental_async::Authorization(::grpc::ClientContext* context, const ::blockchain::AuthorizationRequest* request, ::blockchain::AuthorizationResponse* response, ::grpc::experimental::ClientUnaryReactor* reactor) {
  ::grpc_impl::internal::ClientCallbackUnaryFactory::Create(stub_->channel_.get(), stub_->rpcmethod_Authorization_, context, request, response, reactor);
}

void Blockchain::Stub::experimental_async::Authorization(::grpc::ClientContext* context, const ::grpc::ByteBuffer* request, ::blockchain::AuthorizationResponse* response, ::grpc::experimental::ClientUnaryReactor* reactor) {
  ::grpc_impl::internal::ClientCallbackUnaryFactory::Create(stub_->channel_.get(), stub_->rpcmethod_Authorization_, context, request, response, reactor);
}

::grpc::ClientAsyncResponseReader< ::blockchain::AuthorizationResponse>* Blockchain::Stub::AsyncAuthorizationRaw(::grpc::ClientContext* context, const ::blockchain::AuthorizationRequest& request, ::grpc::CompletionQueue* cq) {
  return ::grpc_impl::internal::ClientAsyncResponseReaderFactory< ::blockchain::AuthorizationResponse>::Create(channel_.get(), cq, rpcmethod_Authorization_, context, request, true);
}

::grpc::ClientAsyncResponseReader< ::blockchain::AuthorizationResponse>* Blockchain::Stub::PrepareAsyncAuthorizationRaw(::grpc::ClientContext* context, const ::blockchain::AuthorizationRequest& request, ::grpc::CompletionQueue* cq) {
  return ::grpc_impl::internal::ClientAsyncResponseReaderFactory< ::blockchain::AuthorizationResponse>::Create(channel_.get(), cq, rpcmethod_Authorization_, context, request, false);
}

::grpc::Status Blockchain::Stub::AddMoney(::grpc::ClientContext* context, const ::blockchain::AddMoneyRequest& request, ::blockchain::AddMoneyResponse* response) {
  return ::grpc::internal::BlockingUnaryCall(channel_.get(), rpcmethod_AddMoney_, context, request, response);
}

void Blockchain::Stub::experimental_async::AddMoney(::grpc::ClientContext* context, const ::blockchain::AddMoneyRequest* request, ::blockchain::AddMoneyResponse* response, std::function<void(::grpc::Status)> f) {
  ::grpc_impl::internal::CallbackUnaryCall(stub_->channel_.get(), stub_->rpcmethod_AddMoney_, context, request, response, std::move(f));
}

void Blockchain::Stub::experimental_async::AddMoney(::grpc::ClientContext* context, const ::grpc::ByteBuffer* request, ::blockchain::AddMoneyResponse* response, std::function<void(::grpc::Status)> f) {
  ::grpc_impl::internal::CallbackUnaryCall(stub_->channel_.get(), stub_->rpcmethod_AddMoney_, context, request, response, std::move(f));
}

void Blockchain::Stub::experimental_async::AddMoney(::grpc::ClientContext* context, const ::blockchain::AddMoneyRequest* request, ::blockchain::AddMoneyResponse* response, ::grpc::experimental::ClientUnaryReactor* reactor) {
  ::grpc_impl::internal::ClientCallbackUnaryFactory::Create(stub_->channel_.get(), stub_->rpcmethod_AddMoney_, context, request, response, reactor);
}

void Blockchain::Stub::experimental_async::AddMoney(::grpc::ClientContext* context, const ::grpc::ByteBuffer* request, ::blockchain::AddMoneyResponse* response, ::grpc::experimental::ClientUnaryReactor* reactor) {
  ::grpc_impl::internal::ClientCallbackUnaryFactory::Create(stub_->channel_.get(), stub_->rpcmethod_AddMoney_, context, request, response, reactor);
}

::grpc::ClientAsyncResponseReader< ::blockchain::AddMoneyResponse>* Blockchain::Stub::AsyncAddMoneyRaw(::grpc::ClientContext* context, const ::blockchain::AddMoneyRequest& request, ::grpc::CompletionQueue* cq) {
  return ::grpc_impl::internal::ClientAsyncResponseReaderFactory< ::blockchain::AddMoneyResponse>::Create(channel_.get(), cq, rpcmethod_AddMoney_, context, request, true);
}

::grpc::ClientAsyncResponseReader< ::blockchain::AddMoneyResponse>* Blockchain::Stub::PrepareAsyncAddMoneyRaw(::grpc::ClientContext* context, const ::blockchain::AddMoneyRequest& request, ::grpc::CompletionQueue* cq) {
  return ::grpc_impl::internal::ClientAsyncResponseReaderFactory< ::blockchain::AddMoneyResponse>::Create(channel_.get(), cq, rpcmethod_AddMoney_, context, request, false);
}

::grpc::Status Blockchain::Stub::InfoBalance(::grpc::ClientContext* context, const ::blockchain::InfoBalanceRequest& request, ::blockchain::InfoBalanceResponse* response) {
  return ::grpc::internal::BlockingUnaryCall(channel_.get(), rpcmethod_InfoBalance_, context, request, response);
}

void Blockchain::Stub::experimental_async::InfoBalance(::grpc::ClientContext* context, const ::blockchain::InfoBalanceRequest* request, ::blockchain::InfoBalanceResponse* response, std::function<void(::grpc::Status)> f) {
  ::grpc_impl::internal::CallbackUnaryCall(stub_->channel_.get(), stub_->rpcmethod_InfoBalance_, context, request, response, std::move(f));
}

void Blockchain::Stub::experimental_async::InfoBalance(::grpc::ClientContext* context, const ::grpc::ByteBuffer* request, ::blockchain::InfoBalanceResponse* response, std::function<void(::grpc::Status)> f) {
  ::grpc_impl::internal::CallbackUnaryCall(stub_->channel_.get(), stub_->rpcmethod_InfoBalance_, context, request, response, std::move(f));
}

void Blockchain::Stub::experimental_async::InfoBalance(::grpc::ClientContext* context, const ::blockchain::InfoBalanceRequest* request, ::blockchain::InfoBalanceResponse* response, ::grpc::experimental::ClientUnaryReactor* reactor) {
  ::grpc_impl::internal::ClientCallbackUnaryFactory::Create(stub_->channel_.get(), stub_->rpcmethod_InfoBalance_, context, request, response, reactor);
}

void Blockchain::Stub::experimental_async::InfoBalance(::grpc::ClientContext* context, const ::grpc::ByteBuffer* request, ::blockchain::InfoBalanceResponse* response, ::grpc::experimental::ClientUnaryReactor* reactor) {
  ::grpc_impl::internal::ClientCallbackUnaryFactory::Create(stub_->channel_.get(), stub_->rpcmethod_InfoBalance_, context, request, response, reactor);
}

::grpc::ClientAsyncResponseReader< ::blockchain::InfoBalanceResponse>* Blockchain::Stub::AsyncInfoBalanceRaw(::grpc::ClientContext* context, const ::blockchain::InfoBalanceRequest& request, ::grpc::CompletionQueue* cq) {
  return ::grpc_impl::internal::ClientAsyncResponseReaderFactory< ::blockchain::InfoBalanceResponse>::Create(channel_.get(), cq, rpcmethod_InfoBalance_, context, request, true);
}

::grpc::ClientAsyncResponseReader< ::blockchain::InfoBalanceResponse>* Blockchain::Stub::PrepareAsyncInfoBalanceRaw(::grpc::ClientContext* context, const ::blockchain::InfoBalanceRequest& request, ::grpc::CompletionQueue* cq) {
  return ::grpc_impl::internal::ClientAsyncResponseReaderFactory< ::blockchain::InfoBalanceResponse>::Create(channel_.get(), cq, rpcmethod_InfoBalance_, context, request, false);
}

Blockchain::Service::Service() {
  AddMethod(new ::grpc::internal::RpcServiceMethod(
      Blockchain_method_names[0],
      ::grpc::internal::RpcMethod::NORMAL_RPC,
      new ::grpc::internal::RpcMethodHandler< Blockchain::Service, ::blockchain::TransactionRequest, ::blockchain::TransactionResponse>(
          [](Blockchain::Service* service,
             ::grpc_impl::ServerContext* ctx,
             const ::blockchain::TransactionRequest* req,
             ::blockchain::TransactionResponse* resp) {
               return service->Transaction(ctx, req, resp);
             }, this)));
  AddMethod(new ::grpc::internal::RpcServiceMethod(
      Blockchain_method_names[1],
      ::grpc::internal::RpcMethod::NORMAL_RPC,
      new ::grpc::internal::RpcMethodHandler< Blockchain::Service, ::blockchain::RegistrationRequest, ::blockchain::RegistrationResponse>(
          [](Blockchain::Service* service,
             ::grpc_impl::ServerContext* ctx,
             const ::blockchain::RegistrationRequest* req,
             ::blockchain::RegistrationResponse* resp) {
               return service->Registration(ctx, req, resp);
             }, this)));
  AddMethod(new ::grpc::internal::RpcServiceMethod(
      Blockchain_method_names[2],
      ::grpc::internal::RpcMethod::NORMAL_RPC,
      new ::grpc::internal::RpcMethodHandler< Blockchain::Service, ::blockchain::AuthorizationRequest, ::blockchain::AuthorizationResponse>(
          [](Blockchain::Service* service,
             ::grpc_impl::ServerContext* ctx,
             const ::blockchain::AuthorizationRequest* req,
             ::blockchain::AuthorizationResponse* resp) {
               return service->Authorization(ctx, req, resp);
             }, this)));
  AddMethod(new ::grpc::internal::RpcServiceMethod(
      Blockchain_method_names[3],
      ::grpc::internal::RpcMethod::NORMAL_RPC,
      new ::grpc::internal::RpcMethodHandler< Blockchain::Service, ::blockchain::AddMoneyRequest, ::blockchain::AddMoneyResponse>(
          [](Blockchain::Service* service,
             ::grpc_impl::ServerContext* ctx,
             const ::blockchain::AddMoneyRequest* req,
             ::blockchain::AddMoneyResponse* resp) {
               return service->AddMoney(ctx, req, resp);
             }, this)));
  AddMethod(new ::grpc::internal::RpcServiceMethod(
      Blockchain_method_names[4],
      ::grpc::internal::RpcMethod::NORMAL_RPC,
      new ::grpc::internal::RpcMethodHandler< Blockchain::Service, ::blockchain::InfoBalanceRequest, ::blockchain::InfoBalanceResponse>(
          [](Blockchain::Service* service,
             ::grpc_impl::ServerContext* ctx,
             const ::blockchain::InfoBalanceRequest* req,
             ::blockchain::InfoBalanceResponse* resp) {
               return service->InfoBalance(ctx, req, resp);
             }, this)));
}

Blockchain::Service::~Service() {
}

::grpc::Status Blockchain::Service::Transaction(::grpc::ServerContext* context, const ::blockchain::TransactionRequest* request, ::blockchain::TransactionResponse* response) {
  (void) context;
  (void) request;
  (void) response;
  return ::grpc::Status(::grpc::StatusCode::UNIMPLEMENTED, "");
}

::grpc::Status Blockchain::Service::Registration(::grpc::ServerContext* context, const ::blockchain::RegistrationRequest* request, ::blockchain::RegistrationResponse* response) {
  (void) context;
  (void) request;
  (void) response;
  return ::grpc::Status(::grpc::StatusCode::UNIMPLEMENTED, "");
}

::grpc::Status Blockchain::Service::Authorization(::grpc::ServerContext* context, const ::blockchain::AuthorizationRequest* request, ::blockchain::AuthorizationResponse* response) {
  (void) context;
  (void) request;
  (void) response;
  return ::grpc::Status(::grpc::StatusCode::UNIMPLEMENTED, "");
}

::grpc::Status Blockchain::Service::AddMoney(::grpc::ServerContext* context, const ::blockchain::AddMoneyRequest* request, ::blockchain::AddMoneyResponse* response) {
  (void) context;
  (void) request;
  (void) response;
  return ::grpc::Status(::grpc::StatusCode::UNIMPLEMENTED, "");
}

::grpc::Status Blockchain::Service::InfoBalance(::grpc::ServerContext* context, const ::blockchain::InfoBalanceRequest* request, ::blockchain::InfoBalanceResponse* response) {
  (void) context;
  (void) request;
  (void) response;
  return ::grpc::Status(::grpc::StatusCode::UNIMPLEMENTED, "");
}


}  // namespace blockchain

