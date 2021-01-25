#pragma once
#include "rest_controller.hpp"

using namespace prails::utilities;

template <class T>
using JsonDecorator = std::function<void(T &, nlohmann::json &)>;

template <class T>
nlohmann::json ColumnValueToJson(std::optional<T> col) {
  if (col.has_value()) {
    if constexpr(std::is_same_v<T, std::tm>)
      return prails::utilities::tm_to_iso8601(col.value());
    else
      return col.value();
  }

  return  nullptr;
}

// TODO: Can we replace T with https://stackoverflow.com/questions/30930350/why-member-functions-cant-be-used-as-template-arguments
template<typename T, class U, class V>
JsonDecorator<V> with(
std::vector<V> &records, const std::string json_key, const std::string foreign_key_attr) {
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

  std::map<T, U> id_to_foreign_model; 
  for(auto &sr: U::Select(query, params))
    id_to_foreign_model[std::get<T>(*sr.recordGet("id"))] = sr;

  return [=](V &m, nlohmann::json &obj) mutable { 
    obj[json_key] = nullptr;
    if (m.recordGet(foreign_key_attr).has_value()) { 
      T foreign_id = std::get<T>(m.recordGet(foreign_key_attr).value());
      if (id_to_foreign_model.count(foreign_id))
        obj[json_key] = id_to_foreign_model[foreign_id].to_json();
    }
  };
}

template <class T, class V>
nlohmann::json ModelToJson(std::vector<T> &models, std::vector<JsonDecorator<V>> decorators) {
  auto ret = nlohmann::json::array();

  std::transform(models.begin(), models.end(), back_inserter(ret), 
    [&decorators](auto& model) { 
      auto js = model.to_json();

      std::for_each(decorators.begin(), decorators.end(), 
        [&model, &js](const auto &decorator) { decorator(model, js); } );
      return js;
    });

  return ret;
}
