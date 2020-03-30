/*
This file is part of bt-trx

bt-trx is free software: you can redistribute it and/or modify
it under the terms of the GNU General Public License as published by
the Free Software Foundation, either version 3 of the License, or
(at your option) any later version.

bt-trx is distributed in the hope that it will be useful,
but WITHOUT ANY WARRANTY; without even the implied warranty of
MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
GNU General Public License for more details.

You should have received a copy of the GNU General Public License
along with this program.  If not, see <http://www.gnu.org/licenses/>.

Copyright (C) 2019 Christian Obersteiner (DL1COM), Andreas Müller (DC1MIL)
Contact: bt-trx.com, mail@bt-trx.com
*/

#ifdef ARDUINO

#include "bttrx_wifi.h"

void BTTRX_WIFI::onRequest(AsyncWebServerRequest *request) {
  // Handle Unknown Request
  request->send(404);
}

String BTTRX_WIFI::resultPage(uint8_t update_error) {
  String resultString = "";
  if (update_error) {
    resultString += "FAILED, Error: ";
    resultString += updateErrorcodeToString(update_error);
  } else {
    resultString += "OK";
  }

  String website = update_result_html;
  website.replace("%STYLE%", style_css);
  website.replace("%RESULT%", resultString);
  return website;
}

String BTTRX_WIFI::updateErrorcodeToString(uint8_t update_error) {
  String return_value = "";
  switch (update_error) {
  case UPDATE_ERROR_OK:
    return_value = "UPDATE_OK";
    break;
  case UPDATE_ERROR_WRITE:
  case UPDATE_ERROR_ERASE:
  case UPDATE_ERROR_READ:
    return_value = "FLASH_ERROR";
    break;
  case UPDATE_ERROR_SPACE:
  case UPDATE_ERROR_NO_PARTITION:
    return_value = "NOT_ENOUGH_SPACE";
    break;
  case UPDATE_ERROR_SIZE: // new firmware size is zero
  case UPDATE_ERROR_STREAM:
  case UPDATE_ERROR_MAGIC_BYTE: // new firmware does not have 0xE9 in first byte
    return_value = "BAD_FIRMWARE_IMAGE";
    break;
  case UPDATE_ERROR_MD5:
    return_value = "BAD_CHECKSUM";
    break;
  case UPDATE_ERROR_ACTIVATE:
  case UPDATE_ERROR_BAD_ARGUMENT:
  case UPDATE_ERROR_ABORT:
    return_value = "OTHER_ERROR";
    break;
  default:
    return_value = "UNKNOWN_ERROR";
    break;
  }
  return return_value + " (" + update_error + ")";
}

void BTTRX_WIFI::firmwareUpdateResponse(AsyncWebServerRequest *request) {
  AsyncWebServerResponse *response =
      request->beginResponse(200, "text/html", resultPage(Update.getError()));
  response->addHeader("Connection", "close");
  request->send(response);
  delay(3000);
  Serial.println("rebooting");
  ESP.restart();
}

void BTTRX_WIFI::handleSet(AsyncWebServerRequest *request) {
  string name = "";
  string value = "";
  if (request->hasParam("id")) {
    name = request->getParam("id")->value().c_str();
  }
  if (request->hasParam("value")) {
    value = request->getParam("value")->value().c_str();
  }

  if (name.empty() || value.empty()) {
    // TODO Send Error to Website
    return;
  }

  if (bttrx_control_->set(name, value) == kSuccess) {
    request->send(200, "text/plain", "Setting changed");
  } else {
    request->send(500, "text/plain", "Error");
  }
}

void BTTRX_WIFI::handleGet(AsyncWebServerRequest *request) {
  string name = "";
  if (request->hasParam("id")) {
    name = request->getParam("id")->value().c_str();
  }

  if (name.empty()) {
    // TODO Send Error to Website
    return;
  }

  string value = "";
  if (bttrx_control_->get(name, &value) == kSuccess) {
    request->send(200, "text/plain", value.c_str());
  } else {
    request->send(500, "text/plain", "Error");
  }
}

void BTTRX_WIFI::handleAction(AsyncWebServerRequest *request) {
  string name = "";
  if (request->hasParam("id")) {
    name = request->getParam("id")->value().c_str();
  }
  if (name.empty()) {
    // TODO Send Error to Website
    return;
  }

  if (bttrx_control_->action(name) == kSuccess) {
    request->send(200, "text/plain", "Success");
    return;
  }
  request->send(500, "text/plain", "Error");
}

void BTTRX_WIFI::setup(BTTRX_CONTROL *control) {
  if (control == nullptr) {
    Serial.println("nullptr given");
    return;
  }
  bttrx_control_ = control;

  Serial.println("Configuring access point...");

  WiFi.softAP(WIFI_SSID, WIFI_PASSWORD);
  IPAddress myIP = WiFi.softAPIP();
  Serial.print("AP IP address: ");
  Serial.println(myIP);

  /*use mdns for host name resolution*/
  if (!MDNS.begin(WIFI_HOSTNAME)) { // http://bt-trx.local
    Serial.println("Error setting up MDNS responder!");
    while (1) {
      delay(1000);
    }
  }
  Serial.println("mDNS responder started");

  // respond to GET requests on URL /heap
  server.on("/heap", HTTP_GET, [](AsyncWebServerRequest *request) {
    request->send(200, "text/plain", String(ESP.getFreeHeap()));
  });

  /*return index page which is stored in serverIndex */
  server.on("/", HTTP_GET, [](AsyncWebServerRequest *request) {
    String website = index_html;
    website.replace("%STYLE%", style_css);
    website.replace("%SCRIPT%", script_js);
    website.replace("%GIT_REVISION%", GIT_REVISION);
    AsyncWebServerResponse *response =
        request->beginResponse(200, "text/html", website);
    response->addHeader("Connection", "close");
    request->send(response);
  });
  /*handling uploading firmware file */
  server.on(
      "/update", HTTP_POST,
      std::bind(&BTTRX_WIFI::firmwareUpdateResponse, this,
                std::placeholders::_1),
      [](AsyncWebServerRequest *request, String filename, size_t index,
         uint8_t *data, size_t len, bool final) {
        if (!index) {
          Serial.printf("Update Start: %s\n", filename.c_str());
          if (!Update.begin((ESP.getFreeSketchSpace() - 0x1000) & 0xFFFFF000)) {
            Update.printError(Serial);
          }
        }
        if (!Update.hasError()) {
          if (Update.write(data, len) != len) {
            Update.printError(Serial);
          }
        }
        if (final) {
          if (Update.end(true)) {
            Serial.printf("Update Success: %uB\n", index + len);
          } else {
            Update.printError(Serial);
          }
        }
      });

  server.on("/set", HTTP_GET,
            std::bind(&BTTRX_WIFI::handleSet, this, std::placeholders::_1));

  server.on("/get", HTTP_GET,
            std::bind(&BTTRX_WIFI::handleGet, this, std::placeholders::_1));

  server.on("/action", HTTP_GET,
            std::bind(&BTTRX_WIFI::handleAction, this, std::placeholders::_1));

  // Catch-All Handler
  // Any request that can not find a Handler that canHandle it
  // ends in the callbacks below.
  server.onNotFound(
      std::bind(&BTTRX_WIFI::onRequest, this, std::placeholders::_1));

  server.begin();
  MDNS.addService("http", "tcp", 80);

  Serial.printf("Ready! Open http://%s.local in your browser\n", WIFI_HOSTNAME);
};

#endif // ARDUINO
