#pragma once
#include "controller.hpp"
#include "controller_factory.hpp"

class AuthController : public Controller::Instance { 
  public:
    using Instance::Instance;
    static void Routes(Pistache::Rest::Router&, std::shared_ptr<Controller::Instance>);

    Controller::Response user_show(const Pistache::Rest::Request&);
    Controller::Response user_update(const Pistache::Rest::Request&);
    Controller::Response user_password(const Pistache::Rest::Request&);
    Controller::Response options(const Pistache::Rest::Request&);
    Controller::Response login(const Pistache::Rest::Request&);
    Controller::Response logout(const Pistache::Rest::Request&);

		template <typename T>                                                                   
		T ensure_authorization(const Pistache::Rest::Request &request) {                        
			auto authorization = request.headers().get<Pistache::Http::Header::Authorization>();  
			std::smatch matches;                                                                  
																																														
			if ((!authorization) ||                                                               
				(!authorization->hasMethod<Pistache::Http::Header::Authorization::Method::Bearer>())
			) throw RequestException("This resource requires a valid authorization header");      
																																														
			std::string authorization_value = authorization->value();                             
			if (!std::regex_search(authorization_value, matches, std::regex("^Bearer (.+)$")))    
				throw RequestException("This resource requires a valid authorization header");      
																																														
			auto ret = T::FromToken(matches[1]);                                                  
			if (!ret) throw RequestException("Invalid authorization supplied");                   
																																														
			return ret.value();                                                                   
		}                                                                                       

  private:
    static ControllerRegister<AuthController> reg;
};

