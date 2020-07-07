#include "gtest/gtest.h"
#include "httplib.h"
#include "rapidjson/document.h"

#include "server.hpp"
#include "model.hpp"
#include "task.hpp"

using namespace std;

// TODO: We Should dry this up against the auth
Pistache::Address addr;

httplib::Client get_client() {
  return httplib::Client(addr.host().c_str(), (uint16_t) addr.port());
}

int main(int argc, char **argv) {

  spdlog::set_level(spdlog::level::debug);
  ::testing::InitGoogleTest(&argc, argv);

  ConfigParser config(string(PROJECT_SOURCE_DIR)+"/config/test-server.yml");

  Model::Initialize(config);

  Task::Migrate();

  Server server(config);
  server.startThreaded();

  addr = Pistache::Address(config.address(), config.port());
  
  auto ret = RUN_ALL_TESTS();
  server.shutdown();

  return ret;
}

TEST(task_controller, index) {
  struct tm test_epoch;
  istringstream ss("2020-04-14 16:35:12");
  ss >> get_time(&test_epoch, "%Y-%m-%d %H:%M:%S");

  for( unsigned int i = 0; i < 10; i++ )
    EXPECT_NO_THROW(Task({
      {"name",        "Task "+to_string(i)},
      {"active",      (int) true},
      {"description", "lorem ipsum sit dolor"},
      {"updated_at",  test_epoch},
      {"created_at",  test_epoch}
    }).save());

  EXPECT_EQ(Task::Count("select count(*) from tasks"), 10);

  auto res = get_client().Get("/api/demo/tasks");

  EXPECT_EQ(res->status, 200);

  rapidjson::Document document;
  document.Parse(res->body.c_str());

  EXPECT_TRUE(document.IsArray());
  EXPECT_EQ(document.Size(), 10);

  for (rapidjson::SizeType i = 0; i < document.Size(); i++) {
    EXPECT_EQ(document[i]["active"].GetInt(), 1);
    EXPECT_EQ(document[i]["id"].GetInt(), i+1);
    EXPECT_EQ(string(document[i]["name"].GetString()), "Task "+to_string(i));
    EXPECT_EQ(string(document[i]["description"].GetString()), "lorem ipsum sit dolor");
    EXPECT_EQ(string(document[i]["created_at"].GetString()), "2020-04-14T16:35:12.0+0000");
    EXPECT_EQ(string(document[i]["updated_at"].GetString()), "2020-04-14T16:35:12.0+0000");
    EXPECT_EQ(document[i].MemberCount(), 6);
  }

  for (auto& task : Task::Select("select * from tasks")) task.remove();

  EXPECT_EQ(Task::Count("select count(*) from tasks"), 0);
}

TEST(task_controller, create) {

  // TODO
  /*
  std::string body("body goes here");                      
  res = client.Post("/read/function1", body, "text/plain");
  EXPECT_EQ(res->status, 405);                             
  EXPECT_EQ(res->body, "Method Not Allowed");              
   */
}

// TODO: test show
// TODO: test edit
// TODO: test delete
// TODO: test multi-edit
// TODO: test multi-delete
