#pragma once
#include "rest_controller.hpp"

using namespace prails::utilities;

// TODO:
// This is kind of a temporary idea atm. Probably this should get put into either
// the controller or the model somewhere... Tests would be good...
template<typename T, class U, class V>
std::map<T, U> with(std::vector<V> &records, const std::string & foreign_key_attr) {

  // Let's first collect a vector of the unique fkey values in the provided recordset
  std::vector<T> values;
  for (auto &r: records) {
    Model::RecordValueOpt val = r.recordGet(foreign_key_attr);
		if (val.has_value()) {
      T cast_val = std::get<T>(*val);
      if (find(values.begin(), values.end(), cast_val) == values.end())
        values.push_back(cast_val);
    }
  }

  // Now we'll construct a query
  Model::Record params;
  std::string in_attr_keys;
  for (unsigned int i = 0; i < values.size(); i++) {
    std::string key = "id"+std::to_string(i);
    params[key] = values[i];
    in_attr_keys.append(((i == 0) ? ":" : ", :")+key);
  }

  std::string query = fmt::format(
		"select * from `{table_name}` where `{table_name}`.`id` in ({in_attr_keys})", 
		fmt::arg("table_name", U::Definition.table_name),
		fmt::arg("in_attr_keys", in_attr_keys)
	);

  std::map<T, U> ret; 
  for(auto &sr: U::Select(query, params))
    ret[std::get<T>(*sr.recordGet("id"))] = sr;

  return ret;
}
