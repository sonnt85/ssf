#include <string>

#include <fstream>
#include <boost/property_tree/json_parser.hpp>
#include <boost/system/error_code.hpp>

#include <ssf/log/log.h>

#include "common/config/config.h"
#include "common/error/error.h"

namespace ssf {
namespace config {

Tls::Tls()
    : ca_cert_path_("./certs/trusted/ca.crt"),
      cert_path_("./certs/certificate.crt"),
      key_path_("./certs/private.key"),
      key_password_(""),
      dh_path_("./certs/dh4096.pem"),
      cipher_alg_("DHE-RSA-AES256-GCM-SHA384") {}

void Tls::Log() const {
#ifdef TLS_OVER_TCP_LINK
  SSF_LOG(kLogInfo) << "config[tls]: CA cert path: <" << ca_cert_path_ << ">";
  SSF_LOG(kLogInfo) << "config[tls]: cert path: <" << cert_path_ << ">";
  SSF_LOG(kLogInfo) << "config[tls]: key path: <" << key_path_ << ">";
  SSF_LOG(kLogInfo) << "config[tls]: key password: <" << key_password_ << ">";
  SSF_LOG(kLogInfo) << "config[tls]: dh path: <" << dh_path_ << ">";
  SSF_LOG(kLogInfo) << "config[tls]: cipher suite: <" << cipher_alg_ << ">";
#endif
}

Proxy::Proxy() : http_addr_(""), http_port_("") {}

void Proxy::Log() const {
  if (IsSet()) {
    SSF_LOG(kLogInfo) << "config[proxy]: <" << http_addr_ << ":" << http_port_
                      << ">";
  } else {
    SSF_LOG(kLogInfo) << "config[proxy]: <None>";
  }
}

ProcessService::ProcessService() : path_(SSF_PROCESS_SERVICE_BINARY_PATH) {}

ProcessService::ProcessService(const ProcessService& process_service)
    : path_(process_service.path_) {}

Services::Services() : process_() {}

Services::Services(const Services& services) : process_(services.process_) {}

void Services::UpdateProcessService(const boost::property_tree::ptree& pt) {
  auto process_optional = pt.get_child_optional("process");
  if (!process_optional) {
    SSF_LOG(kLogDebug)
        << "config[update]: process service configuration not found";
    return;
  }

  auto& process_prop = process_optional.get();
  auto path = process_prop.get_child_optional("path");
  if (path) {
    process_.set_path(path.get().data());
  }
}

void Services::Log() const {
  SSF_LOG(kLogInfo) << "config[services][process]: <"
                    << process_service().path() << ">";
}

Config::Config() : tls_(), proxy_(), services_() {}

void Config::Update(const std::string& filepath,
                    boost::system::error_code& ec) {
  std::string conf_file("config.json");
  ec.assign(::error::success, ::error::get_ssf_category());
  if (filepath == "") {
    std::ifstream ifile(conf_file);
    if (!ifile.good()) {
      return;
    }
  } else {
    conf_file = filepath;
  }

  SSF_LOG(kLogInfo) << "config[ssf]: loading file <" << conf_file << ">";

  try {
    boost::property_tree::ptree pt;
    boost::property_tree::read_json(conf_file, pt);

    UpdateTls(pt);
    UpdateProxy(pt);
    UpdateServices(pt);
  } catch (const std::exception& e) {
    SSF_LOG(kLogError) << "config[ssf]: error reading SSF config file: "
                       << e.what();
    ec.assign(::error::invalid_argument, ::error::get_ssf_category());
  }
}

void Config::Log() const {
  tls_.Log();
  proxy_.Log();
  services_.Log();
}

void Config::UpdateTls(const boost::property_tree::ptree& pt) {
  auto tls_optional = pt.get_child_optional("ssf.tls");
  if (!tls_optional) {
    SSF_LOG(kLogDebug) << "config[update]: TLS configuration not found";
    return;
  }

  auto& tls_prop = tls_optional.get();

  auto ca_cert_path_optional = tls_prop.get_child_optional("ca_cert_path");
  if (ca_cert_path_optional) {
    tls_.set_ca_cert_path(ca_cert_path_optional.get().data());
  }

  auto cert_path_optional = tls_prop.get_child_optional("cert_path");
  if (cert_path_optional) {
    tls_.set_cert_path(cert_path_optional.get().data());
  }

  auto key_path_optional = tls_prop.get_child_optional("key_path");
  if (key_path_optional) {
    tls_.set_key_path(key_path_optional.get().data());
  }

  auto key_password_optional = tls_prop.get_child_optional("key_password");
  if (key_password_optional) {
    tls_.set_key_password(key_password_optional.get().data());
  }

  auto dh_path_optional = tls_prop.get_child_optional("dh_path");
  if (dh_path_optional) {
    tls_.set_dh_path(dh_path_optional.get().data());
  }

  auto cipher_alg_optional = tls_prop.get_child_optional("cipher_alg");
  if (cipher_alg_optional) {
    tls_.set_cipher_alg(cipher_alg_optional.get().data());
  }
}

void Config::UpdateProxy(const boost::property_tree::ptree& pt) {
  auto proxy_optional = pt.get_child_optional("ssf.proxy");
  if (!proxy_optional) {
    SSF_LOG(kLogDebug) << "config[update]: proxy configuration not found";
    return;
  }

  auto& proxy_prop = proxy_optional.get();

  auto http_addr_optional = proxy_prop.get_child_optional("http_addr");
  if (http_addr_optional) {
    proxy_.set_http_addr(http_addr_optional.get().data());
  }

  auto http_port_optional = proxy_prop.get_child_optional("http_port");
  if (http_port_optional) {
    proxy_.set_http_port(http_port_optional.get().data());
  }
}

void Config::UpdateServices(const boost::property_tree::ptree& pt) {
  auto services_optional = pt.get_child_optional("ssf.services");
  if (!services_optional) {
    SSF_LOG(kLogDebug) << "config[update]: services configuration not found";
    return;
  }

  services_.UpdateProcessService(services_optional.get());
}

}  // config
}  // ssf
