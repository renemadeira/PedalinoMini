/*  __________           .___      .__  .__                   ___ ________________    ___
 *  \______   \ ____   __| _/____  |  | |__| ____   ____     /  / \__    ___/     \   \  \   
 *   |     ___// __ \ / __ |\__  \ |  | |  |/    \ /  _ \   /  /    |    | /  \ /  \   \  \  
 *   |    |   \  ___// /_/ | / __ \|  |_|  |   |  (  <_> ) (  (     |    |/    Y    \   )  )
 *   |____|    \___  >____ |(____  /____/__|___|  /\____/   \  \    |____|\____|__  /  /  /
 *                 \/     \/     \/             \/           \__\                 \/  /__/
 *                                                                (c) 2018 alf45star
 *                                                        https://github.com/alf45tar/Pedalino
 */

#include <StreamString.h>

#ifdef ARDUINO_ARCH_ESP8266
#include <ESP8266WiFi.h>
#include <ESP8266WebServer.h>
#include <ESP8266HTTPUpdateServer.h>

ESP8266WebServer        httpServer(80);
ESP8266HTTPUpdateServer httpUpdater;
#endif

#ifdef ARDUINO_ARCH_ESP32
#include <WiFi.h>
#include <WiFiClient.h>
#include <WebServer.h>
#include <Update.h>

WebServer               httpServer(80);
#endif

void blynk_refresh();
void blynk_disconnect();

#ifdef WEBCONFIG

String blynk_get_token();
String blynk_set_token(String);
void blynk_connect();
void blynk_disconnect();
void blynk_refresh();


String  theme = "bootstrap";
String  alert = "";
String  bank  = "1";

String get_top_page(byte p = 0) {

  String page = "";

  page += F("<!doctype html>");
  page += F("<html lang='en'>");
  page += F("<head>");
  page += F("<title>Pedalino&trade;</title>");
  page += F("<meta charset='utf-8'>");
  page += F(" <meta name='viewport' content='width=device-width, initial-scale=1, shrink-to-fit=no'>");
  if ( theme == "bootstrap" ) {
    page += F("<link rel='stylesheet' href='https://stackpath.bootstrapcdn.com/bootstrap/4.1.3/css/bootstrap.min.css' integrity='sha384-MCw98/SFnGE8fJT3GXwEOngsV7Zt27NXFoaoApmYm81iuXoPkFOJwJ8ERdknLPMO' crossorigin='anonymous'>");
  } else {
    page += F("<link rel='stylesheet' href='https://maxcdn.bootstrapcdn.com/bootswatch/4.1.3/");
    page += theme;
    page += F("/bootstrap.min.css' crossorigin='anonymous'>");
  }
  page += F("</head>");

  page += F("<body>");
  page += F("<p></p>");
  page += F("<div class='container-fluid'>");

  page += F("<nav class='navbar navbar-expand navbar-light bg-light'>");
  page += F("<a class='navbar-brand' href='/'>Pedalino&trade;</a>");
  page += F("<button class='navbar-toggler' type='button' data-toggle='collapse' data-target='#navbarNavDropdown' aria-controls='navbarNavDropdown' aria-expanded='false' aria-label='Toggle navigation'>");
  page += F("<span class='navbar-toggler-icon'></span>");
  page += F("</button>");
  page += F("<div class='collapse navbar-collapse' id='navbarNavDropdown'>");
  page += F("<ul class='navbar-nav mr-auto'>");
  page += F("<li class='nav-item");
  page += (p == 1 ? F(" active'>") : F("'>"));
  page += F("<a class='nav-link' href='/live'>Live</a>");
  page += F("</li>");
  page += F("<li class='nav-item");
  page += (p == 2 ? F(" active'>") : F("'>"));
  page += F("<a class='nav-link' href='/banks'>Banks</a>");
  page += F("</li>");
  page += F("<li class='nav-item");
  page += (p == 3 ? F(" active'>") : F("'>"));
  page += F("<a class='nav-link' href='/pedals'>Pedals</a>");
  page += F("</li>");
  page += F("<li class='nav-item");
  page += (p == 4 ? F(" active'>") : F("'>"));
  page += F("<a class='nav-link' href='/interfaces'>Interfaces</a>");
  page += F("</li>");
  page += F("<li class='nav-item");
  page += (p == 5 ? F(" active'>") : F("'>"));
  page += F("<a class='nav-link' href='/options'>Options</a>");
  page += F("</li>");
  page += F("</ul>");
  //page += F("<form class='form-inline my-2 my-lg-0'>");
  //page += F("<button class='btn btn-primary my-2 my-sm-0' type='button'>Save</button>");
  //page += F("</form>");
  page += F("</div>");
  page += F("</nav>");

  if (alert != "") {
    page += F("<div class='alert alert-success alert-dismissible fade show' role='alert'>");
    page += alert;
    page += F("<button type='button' class='close' data-dismiss='alert' aria-label='Close'>");
    page += F("<span aria-hidden='true'>&times;</span>");
    page += F("</button>");
    page += F("</div>");
    alert = "";
  }

  return page;
}

String get_footer_page() {

  String page = "";
  page += F("<nav class='navbar fixed-bottom navbar-light bg-light'>");
  page += F("<a class='navbar-text' href='https://github.com/alf45tar/Pedalino'>https://github.com/alf45tar/Pedalino</a>");
  page += F("</nav>");

  page += F("</div>");
  page += F("<script src='https://code.jquery.com/jquery-3.3.1.slim.min.js' integrity='sha384-q8i/X+965DzO0rT7abK41JStQIAqVgRVzpbzo5smXKp4YfRvH+8abtTE1Pi6jizo' crossorigin='anonymous'></script>");
  page += F("<script src='https://cdnjs.cloudflare.com/ajax/libs/popper.js/1.14.3/umd/popper.min.js' integrity='sha384-ZMP7rVo3mIykV+2+9J3UJ46jBk0WLaUAdn689aCwoqbBJiSnjAK/l8WvCWPIPm49' crossorigin='anonymous'></script>");
  page += F("<script src='https://stackpath.bootstrapcdn.com/bootstrap/4.1.3/js/bootstrap.min.js' integrity='sha384-ChfqqxuZUCnJSK3+MXmPNIyE6ZbWh2IMqE241rYiqJxyMiZ6OW/JmZQ5stwEULTy' crossorigin='anonymous'></script>");
  page += F("</body>");
  page += F("</html>");

  return page;
}

String get_root_page() {

  String page = "";

  page += get_top_page();

  page += F("<p></p>");
  page += F("<h6>Smart wireless MIDI foot controller for guitarists and more</h6>");
  page += F("<p></p>");

  page += F("<dl>");
  page += F("<dt>Chip ID</dt><dd>");
#ifdef ARDUINO_ARCH_ESP8266
  page += ESP.getChipId();
#endif
#ifdef ARDUINO_ARCH_ESP32
  page += String((uint32_t)ESP.getEfuseMac());
#endif
  page += F("</dd>");
  page += F("<dt>Flash Chip ID</dt><dd>");
#ifdef ARDUINO_ARCH_ESP8266
  page += ESP.getFlashChipId();
#endif
#ifdef ARDUINO_ARCH_ESP32
  page += String((uint32_t)ESP.getEfuseMac());
#endif
  page += F("</dd>");
  page += F("<dt>Chip Speed</dt><dd>");
  page += ESP.getFlashChipSpeed();
  page += F(" hertz</dd>");
  page += F("<dt>IDE Flash Size</dt><dd>");
  page += ESP.getFlashChipSize();
  page += F(" bytes</dd>");
#ifdef ARDUINO_ARCH_ESP8266 
  page += F("<dt>Real Flash Size</dt><dd>");
  page += ESP.getFlashChipRealSize();
  page += F(" bytes</dd>");
#endif
  page += F("<dt>Soft AP IP</dt><dd>");
  page += WiFi.softAPIP().toString();
  page += F("</dd>");
  page += F("<dt>Soft AP MAC</dt><dd>");
  page += WiFi.softAPmacAddress();
  page += F("</dd>");
  page += F("<dt>Station MAC</dt><dd>");
  page += WiFi.macAddress();
  page += F("</dd>");
  page += F("</dl>");

  page += get_footer_page();

  return page;
}

String get_live_page() {

  String page = "";

  page += get_top_page(1);

  page += F("<p></p>");

  page += get_footer_page();

  return page;
}

String get_banks_page() {

  String page = "";

  page += get_top_page(2);
  /*
    page += "<div class='container-fluid'>";

    page += "<h1>Pedalino&trade;</h1>";
    page += "<div class='row'>";
    page += "<div class='col'>";
    page += "<ul class='nav nav-pills'>";
    page += "<li class='nav-item'><a class='nav-link' href='/live'>Live</a></li>";
    page += "<li class='nav-item active'><a class='nav-link' href='/banks'>Banks</a></li>";
    page += "<li class='nav-item'><a class='nav-link' href='/pedals'>Pedals</a></li>";
    page += "<li class='nav-item'><a class='nav-link' href='/interfaces'>Interfaces</a></li>";
    page += "<li class='nav-item dropdown'>";
    page +=       "<form method='POST' name='selecttheme' id='selecttheme'/>";
    page +=       "<input class='span' id='choixtheme' name='theme' type='hidden'>";
    page += "<a class='nav-link dropdown-toggle' data-toggle='dropdown' href='#'>Theme</a>";
    //page += "<div class='dropdown-menu'>";
    //page += "<a class='dropdown-item' href='#''>Link 1</a>";
    page += "<ul class='dropdown-menu'>";
    page += "<li onclick='$(\"#choixtheme\").val(\"bootstrap\"); $(\"#selecttheme\").submit()'><a href='#'>Boostrap</a></li>";
    page += "<li onclick='$(\"#choixtheme\").val(\"cerulean\"); $(\"#selecttheme\").submit()'><a href='#'>Cerulean</a></li>";
    page += "<li onclick='$(\"#choixtheme\").val(\"cosmo\"); $(\"#selecttheme\").submit()'><a href='#'>Cosmo</a></li>";
    page += "<li onclick='$(\"#choixtheme\").val(\"cyborg\"); $(\"#selecttheme\").submit()'><a href='#'>Cyborg</a></li>";
    page += "<li onclick='$(\"#choixtheme\").val(\"darkly\"); $(\"#selecttheme\").submit()'><a href='#'>Darkly</a></li>";
    page += "<li onclick='$(\"#choixtheme\").val(\"flatly\"); $(\"#selecttheme\").submit()'><a href='#'>Flatly</a></li>";
    page += "<li onclick='$(\"#choixtheme\").val(\"journal\"); $(\"#selecttheme\").submit()'><a href='#'>Journal</a></li>";
    page += "<li onclick='$(\"#choixtheme\").val(\"lumen\"); $(\"#selecttheme\").submit()'><a href='#'>Lumen</a></li>";
    page += "<li onclick='$(\"#choixtheme\").val(\"paper\"); $(\"#selecttheme\").submit()'><a href='#'>Paper</a></li>";
    page += "<li onclick='$(\"#choixtheme\").val(\"readable\"); $(\"#selecttheme\").submit()'><a href='#'>Readable</a></li>";
    page += "<li onclick='$(\"#choixtheme\").val(\"sandstone\"); $(\"#selecttheme\").submit()'><a href='#'>Sandstone</a></li>";
    page += "<li onclick='$(\"#choixtheme\").val(\"simplex\"); $(\"#selecttheme\").submit()'><a href='#'>Simplex</a></li>";
    page += "<li onclick='$(\"#choixtheme\").val(\"slate\"); $(\"#selecttheme\").submit()'><a href='#'>Slate</a></li>";
    page += "<li onclick='$(\"#choixtheme\").val(\"spacelab\"); $(\"#selecttheme\").submit()'><a href='#'>Spacelab</a></li>";
    page += "<li onclick='$(\"#choixtheme\").val(\"superhero\"); $(\"#selecttheme\").submit()'><a href='#'>Superhero</a></li>";
    page += "<li onclick='$(\"#choixtheme\").val(\"united\"); $(\"#selecttheme\").submit()'><a href='#'>United</a></li>";
    page += "<li onclick='$(\"#choixtheme\").val(\"yeti\"); $(\"#selecttheme\").submit()'><a href='#'>Yeti</a></li>";
    page += "</ul>";
    //page += "</div>";
    page +=       "</form>";
    page += "</li>";
    page += "</ul>";
    page += "</div>";
    page += "<div class='col' align='right'><button type='button' class='btn btn-primary'>Apply</button> <button type='button' class='btn btn-primary'>Save</button></div>";
    page += "</div>";
  */
  /*
    page += "<nav class='navbar navbar-expand-lg navbar-light bg-light'>";
    page += "<a class='navbar-brand' href='#'>Pedalino&trade;</a>";
    page += "<button class='navbar-toggler' type='button' data-toggle='collapse' data-target='#navbarNavDropdown' aria-controls='navbarNavDropdown' aria-expanded='false' aria-label='Toggle navigation'>";
    page += "<span class='navbar-toggler-icon'></span>";
    page += "</button>";
    page += "<div class='collapse navbar-collapse' id='navbarNavDropdown'>";
    page += "<ul class='navbar-nav'>";
    page += "<li class='nav-item'>";
    page += "<a class='nav-link' href='/'>Home <span class='sr-only'>(current)</span></a>";
    page += "</li>";
    page += "<li class='nav-item'>";
    page += "<a class='nav-link' href='/live'>Live</a>";
    page += "</li>";
    page += "<li class='nav-item active'>";
    page += "<a class='nav-link' href='/banks'>Banks</a>";
    page += "</li>";
    page += "<li class='nav-item'>";
    page += "<a class='nav-link' href='/pedals'>Pedals</a>";
    page += "</li>";
    page += "<li class='nav-item dropdown'>";
    page += "<form method='POST' name='selecttheme' id='selecttheme'/>";
    page += "<input class='span' id='choixtheme' name='theme' type='hidden'>";
    page += "<a class='nav-link dropdown-toggle' href='#' id='navbarDropdownMenuLink' data-toggle='dropdown' aria-haspopup='true' aria-expanded='false'>Theme</a>";
    page += "<div class='dropdown-menu' aria-labelledby='navbarDropdownMenuLink'>";
    page += "<a class='dropdown-item' onclick='$(\"#choixtheme\").val(\"bootstrap\"); $(\"#selecttheme\").submit()' href='#'>Boostrap</a>";
    page += "<a class='dropdown-item' onclick='$(\"#choixtheme\").val(\"cerulean\"); $(\"#selecttheme\").submit()' href='#'>Cerulean</a>";
    page += "<a class='dropdown-item' onclick='$(\"#choixtheme\").val(\"cosmo\"); $(\"#selecttheme\").submit()' href='#'>Cosmo</a>";
    page += "<a class='dropdown-item' onclick='$(\"#choixtheme\").val(\"cyborg\"); $(\"#selecttheme\").submit()' href='#'>Cyborg</a>";
    page += "<a class='dropdown-item' onclick='$(\"#choixtheme\").val(\"darkly\"); $(\"#selecttheme\").submit()' href='#'>Darkly</a>";
    page += "<a class='dropdown-item' onclick='$(\"#choixtheme\").val(\"flatly\"); $(\"#selecttheme\").submit()' href='#'>Flatly</a>";
    page += "<a class='dropdown-item' onclick='$(\"#choixtheme\").val(\"journal\"); $(\"#selecttheme\").submit()' href='#'>Journal</a>";
    page += "<a class='dropdown-item' onclick='$(\"#choixtheme\").val(\"lumen\"); $(\"#selecttheme\").submit()' href='#'>Lumen</a>";
    page += "<a class='dropdown-item' onclick='$(\"#choixtheme\").val(\"paper\"); $(\"#selecttheme\").submit()' href='#'>Paper</a>";
    page += "<a class='dropdown-item' onclick='$(\"#choixtheme\").val(\"readable\"); $(\"#selecttheme\").submit()' href='#'>Readable</a>";
    page += "<a class='dropdown-item' onclick='$(\"#choixtheme\").val(\"sandstone\"); $(\"#selecttheme\").submit()' href='#'>Sandstone</a>";
    page += "<a class='dropdown-item' onclick='$(\"#choixtheme\").val(\"simplex\"); $(\"#selecttheme\").submit()' href='#'>Simplex</a>";
    page += "<a class='dropdown-item' onclick='$(\"#choixtheme\").val(\"slate\"); $(\"#selecttheme\").submit()' href='#'>Slate</a>";
    page += "<a class='dropdown-item' onclick='$(\"#choixtheme\").val(\"spacelab\"); $(\"#selecttheme\").submit()' href='#'>Spacelab</a>";
    page += "<a class='dropdown-item' onclick='$(\"#choixtheme\").val(\"superhero\"); $(\"#selecttheme\").submit()' href='#'>Superhero</a>";
    page += "<a class='dropdown-item' onclick='$(\"#choixtheme\").val(\"united\"); $(\"#selecttheme\").submit()' href='#'>United</a>";
    page += "<a class='dropdown-item' onclick='$(\"#choixtheme\").val(\"yeti\"); $(\"#selecttheme\").submit()' href='#'>Yeti</a>";
    page += "</div>";
    page += "</form>";
    page += "</li>";
    page += "</ul>";
    page += "<form class='form-inline'>";
    page += "<button class='btn btn-primary' type='button'>Apply</button>";
    page += "<button class='btn btn-primary' type='button'>Save</button>";
    page += "</form>";
    page += "</div>";
    page += "</nav>";
  */

  page += F( "<div class='btn-group'>");
  page += F( "<form><button type='button submit' class='btn ");
  page += (bank == "1" ? String("btn-primary") : String("")) + F("' name='bank' value='1'>1</button></form>");
  page += F( "<form><button type='button submit' class='btn ");
  page += (bank == "2" ? String("btn-primary") : String("")) + F("' name='bank' value='2'>2</button></form>");
  page += F( "<form><button type='button submit' class='btn ");
  page += (bank == "3" ? String("btn-primary") : String("")) + F("' name='bank' value='3'>3</button></form>");
  page += F( "<form><button type='button submit' class='btn ");
  page += (bank == "4" ? String("btn-primary") : String("")) + F("' name='bank' value='4'>4</button></form>");
  page += F( "<form><button type='button submit' class='btn ");
  page += (bank == "5" ? String("btn-primary") : String("")) + F("' name='bank' value='5'>5</button></form>");
  page += F( "<form><button type='button submit' class='btn ");
  page += (bank == "6" ? String("btn-primary") : String("")) + F("' name='bank' value='6'>6</button></form>");
  page += F( "<form><button type='button submit' class='btn ");
  page += (bank == "7" ? String("btn-primary") : String("")) + F("' name='bank' value='7'>7</button></form>");
  page += F("<form><button type='button submit' class='btn ");
  page += (bank == "8" ? String("btn-primary") : String("")) + F("' name='bank' value='8'>8</button></form>");
  page += F("<form><button type='button submit' class='btn ");
  page += (bank == "9" ? String("btn-primary") : String("")) + F("' name='bank' value='9'>9</button></form>");
  page += F("<form><button type='button submit' class='btn ");
  page += (bank == "10" ? String("btn-primary") : String("")) + F("' name='bank' value='10'>10</button></form>");
  page += F("</div>");

  page += F("<table class='table-responsive-sm table-borderless'>");
  page += F("<tbody><tr><td>Pedal</td><td>Message</td><td>Channel</td><td>Code</td><td>Value 1</td><td>Value 2</td><td>Value 3</td></tr>");
  for (unsigned int i = 1; i <= PEDALS; i++) {
    page += F("<tr align='center' valign='center'>");

    page += F("<td>");
    page += String(i);
    page += F("</td>");

    page += F("<td><div class='form-group'>");
    page += F("<select class='custom-select-sm' id='message");
    page += String(i);
    page += F("'>");
    page += F("<option>Program Change</option>");
    page += F("<option>Control Change</option>");
    page += F("<option>Note On/Off</option>");
    page += F("<option>Pitch Bend</option>");
    page += F("</select>");
    page += F("</div></td>");

    page += F("<td><div class='form-group'>");
    page += F("<select class='custom-select-sm' id='channel");
    page += String(i);
    page += F("'>");
    page += F("<option>1</option>");
    page += F("<option>2</option>");
    page += F("<option>3</option>");
    page += F("<option>4</option>");
    page += F("<option>5</option>");
    page += F("<option>6</option>");
    page += F("<option>7</option>");
    page += F("<option>8</option>");
    page += F("<option>9</option>");
    page += F("<option>10</option>");
    page += F("<option>11</option>");
    page += F("<option>12</option>");
    page += F("<option>13</option>");
    page += F("<option>14</option>");
    page += F("<option>15</option>");
    page += F("<option>16</option>");
    page += F("</select>");
    page += F("</div></td>");

    page += F("<td><div class='form-group'>");
    page += F("<input type='number' class='custom-select-sm' name='code' min='0' max='127'>");
    page += F("</div></td>");

    page += F("<td><div class='form-group'>");
    page += F("<input type='number' class='custom-select-sm' name='code1' min='0' max='127'>");
    page += F("</div></td>");

    page += F("<td><div class='form-group'>");
    page += F("<input type='number' class='custom-select-sm' name='code2' min='0' max='127'>");
    page += F("</div></td>");

    page += F("<td><div class='form-group'>");
    page += F("<input type='number' class='custom-select-sm' name='code3' min='0' max='127'>");
    page += F("</div></td>");

    page += F("</tr>");
  }
  page += F("</tbody>");
  page += F("</table>");

  page += get_footer_page();

  return page;
}

String get_pedals_page() {

  String page = "";

  page += get_top_page(3);

  page += F("<p></p>");
  page += F("<form method='post'>");
  page += F("<div class='form-row align-items-center'>");
  page += F("<div class='col-1'>");
  page += F("<span class='badge badge-primary'>Pedal<br>#</span>");
  page += F("</div>");
  page += F("<div class='col-1'>");
  page += F("<span class='badge badge-primary'>Auto<br>Sensing</span>");
  page += F("</div>");
  page += F("<div class='col-2'>");
  page += F("<span class='badge badge-primary'>Pedal<br>Mode</span>");
  page += F("</div>");
  page += F("<div class='col-1'>");
  page += F("<span class='badge badge-primary'>Pedal<br>Function</span>");
  page += F("</div>");
  page += F("<div class='col-1'>");
  page += F("<span class='badge badge-primary'>Single<br>Press</span>");
  page += F("</div>");
  page += F("<div class='col-1'>");
  page += F("<span class='badge badge-primary'>Double<br>Press</span>");
  page += F("</div>");
  page += F("<div class='col-1'>");
  page += F("<span class='badge badge-primary'>Long<br>Press</span>");
  page += F("</div>");
  page += F("<div class='col-1'>");
  page += F("<span class='badge badge-primary'>Invert<br>Polarity</span>");
  page += F("</div>");
  page += F("<div class='col-1'>");
  page += F("<span class='badge badge-primary'>Analog<br>Map</span>");
  page += F("</div>");
  page += F("<div class='col-1'>");
  page += F("<span class='badge badge-primary'>Analog<br>Zero</span>");
  page += F("</div>");
  page += F("<div class='col-1'>");
  page += F("<span class='badge badge-primary'>Analog<br>Max</span>");
  page += F("</div>");
  page += F("</div>");
  page += F("<p></p>");

  for (unsigned int i = 1; i <= PEDALS; i++) {
    page += F("<div class='form-row align-items-center'>");

    page += F("<div class='col-1'>");
    page += String(i);
    page += F("</div>");

    page += F("<div class='col-1'>");
    page += F("<div class='custom-control custom-checkbox'>");
    page += F("<input type='checkbox' class='custom-control-input' id='autoCheck");
    page += String(i) + F("' name='autosensing") + String(i) + F("'");
    if (pedals[i-1].autoSensing) page += F(" checked");
    page += F(">");
    page += F("<label class='custom-control-label' for='autoCheck");
    page += String(i) + F("'></p></label>");
    page += F("</div>");
    page += F("</div>");

    page += F("<div class='col-2'>");
    //page += F("<div class='form-group'>");
    page += F("<select class='custom-select custom-select-sm' id='mode");
    page += String(i);
    page += F("'>");
    page += F("<option value='");
    page += String(PED_NONE) + F("'");
    if (pedals[i-1].mode == PED_NONE) page += F(" selected");
    page += F(">None</option>");
    page += F("<option value='");
    page += String(PED_MOMENTARY1) + F("'");
    if (pedals[i-1].mode == PED_MOMENTARY1) page += F(" selected");
    page += F(">Momentary</option>");
    page += F("<option value='");
    page += String(PED_LATCH1) + F("'");
    if (pedals[i-1].mode == PED_LATCH1) page += F(" selected");
    page += F(">Latch</option>");
    page += F("<option value='");
    page += String(PED_ANALOG) + F("'");
    if (pedals[i-1].mode == PED_ANALOG) page += F(" selected");
    page += F(">Analog</option>");
    page += F("<option value='");
    page += String(PED_JOG_WHEEL) + F("'");
    if (pedals[i-1].mode == PED_JOG_WHEEL) page += F(" selected");
    page += F(">Jog Wheel</option>");
    page += F("<option value='");
    page += String(PED_MOMENTARY2) + F("'");
    if (pedals[i-1].mode == PED_MOMENTARY2) page += F(" selected");
    page += F(">Momentary 2</option>");
    page += F("<option value='");
    page += String(PED_MOMENTARY3) + F("'");
    if (pedals[i-1].mode == PED_MOMENTARY3) page += F(" selected");
    page += F(">Momentary 3</option>");
    page += F("<option value='");
    page += String(PED_LATCH2) + F("'");
    if (pedals[i-1].mode == PED_LATCH2) page += F(" selected");
    page += F(">Latch 2</option>");
    page += F("<option value='");
    page += String(PED_LADDER) + F("'");
    if (pedals[i-1].mode == PED_LADDER) page += F(" selected");
    page += F(">Ladder</option>");
    page += F("</select>");
    //page += F("</div>");
    page += F("</div>");

    page += F("<div class='col-1'>");
    //page += F("<div class='form-group'>");
    page += F("<select class='custom-select custom-select-sm' id='function");
    page += String(i);
    page += F("'>");
    page += F("<option");
    if (pedals[i-1].function == PED_MIDI) page += F(" selected");
    page += F(">MIDI</option>");
    page += F("<option");
    if (pedals[i-1].function == PED_BANK_PLUS) page += F(" selected");
    page += F(">Bank+</option>");
    page += F("<option");
    if (pedals[i-1].function == PED_BANK_MINUS) page += F(" selected");
    page += F(">Bank-</option>");
    page += F("<option");
    if (pedals[i-1].function == PED_START) page += F(" selected");
    page += F(">Start</option>");
    page += F("<option");
    if (pedals[i-1].function == PED_STOP) page += F(" selected");
    page += F(">Stop</option>");
    page += F("<option");
    if (pedals[i-1].function == PED_CONTINUE) page += F(" selected");
    page += F(">Continue</option>");
    page += F("<option");
    if (pedals[i-1].function == PED_CONTINUE) page += F(" selected");
    page += F(">Tap</option>");
    page += F("<option");
    if (pedals[i-1].function == PED_MENU) page += F(" selected");
    page += F(">Menu</option>");
    page += F("<option");
    if (pedals[i-1].function == PED_CONFIRM) page += F(" selected");
    page += F(">Confirm</option>");
    page += F("<option");
    if (pedals[i-1].function == PED_ESCAPE) page += F(" selected");
    page += F(">Excape</option>");
    page += F("<option");
    if (pedals[i-1].function == PED_NEXT) page += F(" selected");
    page += F(">Next</option>");
    page += F("<option");
    if (pedals[i-1].function == PED_PREVIOUS) page += F(" selected");
    page += F(">Previous</option>");
    page += F("</select>");
    //page += F("</div>");
    page += F("</div>");

    page += F("<div class='col-1'>");
    page += F("<div class='custom-control custom-checkbox'>");
    page += F("<input type='checkbox' class='custom-control-input' id='singleCheck");
    page += String(i) + F("' name='singlepress") + String(i) + F("'");
    if (pedals[i-1].pressMode == PED_PRESS_1   ||
        pedals[i-1].pressMode == PED_PRESS_1_2 ||
        pedals[i-1].pressMode == PED_PRESS_1_L ||
        pedals[i-1].pressMode == PED_PRESS_1_2_L) page += F(" checked");
    page += F(">");
    page += F("<label class='custom-control-label' for='singleCheck");
    page += String(i) + F("'></p></label>");
    page += F("</div>");
    page += F("</div>");

    page += F("<div class='col-1'>");
    page += F("<div class='custom-control custom-checkbox'>");
    page += F("<input type='checkbox' class='custom-control-input' id='doubleCheck");
    page += String(i) + F("' name='doublepress") + String(i) + F("'");
    if (pedals[i-1].pressMode == PED_PRESS_2   ||
        pedals[i-1].pressMode == PED_PRESS_1_2 ||
        pedals[i-1].pressMode == PED_PRESS_2_L || 
        pedals[i-1].pressMode == PED_PRESS_1_2_L) page += F(" checked");
    page += F(">");
    page += F("<label class='custom-control-label' for='doubleCheck");
    page += String(i) + F("'></p></label>");
    page += F("</div>");
    page += F("</div>");

    page += F("<div class='col-1'>");
    page += F("<div class='custom-control custom-checkbox'>");
    page += F("<input type='checkbox' class='custom-control-input' id='longCheck");
    page += String(i) + F("' name='longpress") + String(i) + F("'");
    if (pedals[i-1].pressMode == PED_PRESS_L   ||
        pedals[i-1].pressMode == PED_PRESS_1_L ||
        pedals[i-1].pressMode == PED_PRESS_2_L ||
        pedals[i-1].pressMode == PED_PRESS_1_2_L) page += F(" checked");
    page += F(">");
    page += F("<label class='custom-control-label' for='longCheck");
    page += String(i) + F("'></p></label>");
    page += F("</div>");
    page += F("</div>");

    page += F("<div class='col-1'>");
    page += F("<div class='custom-control custom-checkbox'>");
    page += F("<input type='checkbox' class='custom-control-input' id='polarityCheck");
    page += String(i) + F("' name='polarity") + String(i) + F("'");
    if (pedals[i-1].invertPolarity) page += F(" checked");
    page += F(">");
    page += F("<label class='custom-control-label' for='polarityCheck");
    page += String(i) + F("'></p></label>");
    page += F("</div>");
    page += F("</div>");

    page += F("<div class='col-1'>");
    //page += F("<div class='form-group'>");
    page += F("<select class='custom-select custom-select-sm' id='map");
    page += String(i);
    page += F("'>");
    page += F("<option value='");
    page += String(PED_LINEAR) + F("'");
    if (pedals[i-1].mapFunction == PED_LINEAR) page += F(" selected");
    page += F(">Linear</option>");
    page += F("<option value='");
    page += String(PED_LOG) + F("'");
    if (pedals[i-1].mapFunction == PED_LOG) page += F(" selected");
    page += F(">Log</option>");
    page += F("<option value='");
    page += String(PED_ANTILOG) + F("'");
    if (pedals[i-1].mapFunction == PED_ANTILOG) page += F(" selected");
    page += F(">Antilog</option>");  
    page += F("</select>");
    //page += F("</div>");
    page += F("</div>");

    page += F("<div class='col-1'>");
    page += F("<div class='form-group'>");
    page += F("<label for='minControlRange");
    page += String(i) + F("'></label>");
    page += F("<input type='range' class='form-control-range' id='minControlRange");
    page += String(i) + F("' value='0'>");
    page += F("</div>");
    page += F("</div>");

    page += F("<div class='col-1'>");
    page += F("<div class='form-group'>");
    page += F("<label for='maxControlRange");
    page += String(i) + F("'></label>");
    page += F("<input type='range' class='form-control-range' id='maxControlRange");
    page += String(i) + F("' value='100'>");
    page += F("</div>");
    page += F("</div>");

    page += F("</div>");
  }

  page += F("<div class='form-group row'>");
  page += F("<div class='col-1'>");
  page += F("<button type='submit' class='btn btn-primary'>Save</button>");
  page += F("</div>");
  page += F("</div>");
  page += F("</form>");

  page += get_footer_page();

  return page;
}

String get_interfaces_page() {

  String page = "";

  page += get_top_page(4);

  page += F("<p></p>");
  page += F("<form method='post'>");
  page += F("<div class='row'>");
  for (unsigned int i = 1; i <= INTERFACES; i++) {
    page += F("<div class='col-2'>");
    page += F("<span class='badge badge-primary'>");
    page += interfaces[i-1].name;
    page += F("</span>");
    page += F("</div>");
  }
  page += F("</div>");
  page += F("<p></p>");

  page += F("<div class='form-group row'>");
  for (unsigned int i = 1; i <= INTERFACES; i++) {
    page += F("<div class='col-2'>");
    page += F("<div class='custom-control custom-checkbox'>");
    page += F("<input type='checkbox' class='custom-control-input' id='inCheck");
    page += String(i) + F("' name='in") + String(i) + F("'");
    if (interfaces[i-1].midiIn) page += F(" checked");
    page += F(">");
    page += F("<label class='custom-control-label' for='inCheck");
    page += String(i) + F("'>In</label>");
    page += F("</div>");
    page += F("</div>");
  }
  page += F("</div>");
  page += F("<div class='form-group row'>");
  for (unsigned int i = 1; i <= INTERFACES; i++) {
    page += F("<div class='col-2'>");
    page += F("<div class='custom-control custom-checkbox'>");
    page += F("<input type='checkbox' class='custom-control-input' id='outCheck");
    page += String(i) + F("' name='out") + String(i) + F("'");
    if (interfaces[i-1].midiOut) page += F(" checked");
    page += F(">");
    page += F("<label class='custom-control-label' for='outCheck");
    page += String(i) + F("'>Out</label>");
    page += F("</div>");
    page += F("</div>");
  }
  page += F("</div>");
  page += F("<div class='form-group row'>");
  for (unsigned int i = 1; i <= INTERFACES; i++) {
    page += F("<div class='col-2'>");
    page += F("<div class='custom-control custom-checkbox'>");
    page += F("<input type='checkbox' class='custom-control-input' id='thruCheck");
    page += String(i) + F("' name='thru") + String(i) + F("'");
    if (interfaces[i-1].midiThru) page += F(" checked");
    page += F(">");
    page += F("<label class='custom-control-label' for='thruCheck");
    page += String(i) + F("'>Thru</label>");
    page += F("</div>");
    page += F("</div>");
  }
  page += F("</div>");
  page += F("<div class='form-group row'>");
  for (unsigned int i = 1; i <= INTERFACES; i++) {
    page += F("<div class='col-2'>");
    page += F("<div class='custom-control custom-checkbox'>");
    page += F("<input type='checkbox' class='custom-control-input' id='routingCheck");
    page += String(i) + F("' name='routing") + String(i) + F("'");
    if (interfaces[i-1].midiRouting) page += F(" checked");
    page += F(">");
    page += F("<label class='custom-control-label' for='routingCheck");
    page += String(i) + F("'>Routing</label>");
    page += F("</div>");
    page += F("</div>");
  }
  page += F("</div>");
  page += F("<div class='form-group row'>");
  for (unsigned int i = 1; i <= INTERFACES; i++) {
    page += F("<div class='col-2'>");
    page += F("<div class='custom-control custom-checkbox'>");
    page += F("<input type='checkbox' class='custom-control-input' id='clockCheck");
    page += String(i) + F("' name='clock") + String(i) + F("'");
    if (interfaces[i-1].midiClock) page += F(" checked");
    page += F(">");
    page += F("<label class='custom-control-label' for='clockCheck");
    page += String(i) + F("'>Clock</label>");
    page += F("</div>");
    page += F("</div>");
  }
  page += F("</div>");

  page += F("<div class='form-group row'>");
  page += F("<div class='col-2'>");
  page += F("<button type='submit' class='btn btn-primary'>Save</button>");
  page += F("</div>");
  page += F("</div>");
  page += F("</form>");

  page += get_footer_page();

  return page;
}

String get_options_page() {

  String page = "";

  page += get_top_page(5);

  page += F("<p></p>");
  page += F("<form method='post'>");
  page += F("<div class='form-row'>");
  page += F("<label for='blynkauthtoken' class='col-sm-2 col-form-label'>Blynk Auth Token</label>");
  page += F("<div class='col-sm-10'>");
  page += F("<input class='form-control form-control-sm' type='text' length=32 id='blynkauthtoken' placeholder='Blynk Auth Token is 32 characters long. Copy and paste from email.'>");
  page += F("</div>");
  page += F("<div class='col-sm-2'>");
  page += F("</div>");
  page += F("<div class='col-sm-10'>");
  page += F("Auth Token is a unique identifier which is needed to connect your Pedalino to your smartphone. Every Pedalino will have its own Auth Token. You’ll get Auth Token automatically on your email after Pedalino app clone. You can also copy it manually. Click on devices section and selected required device.");
  page += F("Don’t share your Auth Token with anyone, unless you want someone to have access to your Pedalino.");
  page += F("</div>");
  page += F("</div>");
  page += F("<div class='form-group row'>");
  page += F("<div class='col-2'>");
  page += F("<button type='submit' class='btn btn-primary'>Save</button>");
  page += F("</div>");
  page += F("</div>");
  page += F("</form>");

  page += get_footer_page();

  return page;
}

void http_handle_root() {

  if (httpServer.hasArg("theme")) {
    theme = httpServer.arg("theme");
    httpServer.send(200, "text/html", get_root_page());
  } else if (httpServer.hasArg("bank")) {
    bank = httpServer.arg("bank");
    httpServer.send(200, "text/html", get_banks_page());
  } else {
    httpServer.send(200, "text/html", get_root_page());
  }
  return;
}

void http_handle_live() {
  if (httpServer.hasArg("theme")) theme = httpServer.arg("theme");
  httpServer.send(200, "text/html", get_live_page());
}

void http_handle_banks() {
  if (httpServer.hasArg("theme")) theme = httpServer.arg("theme");
  if (httpServer.hasArg("bank"))  bank  = httpServer.arg("bank");
  httpServer.send(200, "text/html", get_banks_page());
}

void http_handle_pedals() {
  if (httpServer.hasArg("theme")) theme = httpServer.arg("theme");
  httpServer.send(200, "text/html", get_pedals_page());
}

void http_handle_interfaces() {
  if (httpServer.hasArg("theme")) theme = httpServer.arg("theme");
  httpServer.sendHeader("Connection", "close");
  httpServer.send(200, "text/html", get_interfaces_page());
}

void http_handle_options() {
  if (httpServer.hasArg("theme")) theme = httpServer.arg("theme");
  httpServer.send(200, "text/html", get_options_page());
}

void http_handle_post_pedals() {
  
  String a;
  String checked("on");
  
  //httpServer.sendHeader("Connection", "close");
  for (unsigned int i = 0; i < PEDALS; i++) {
    a = httpServer.arg(String("autosensing") + String(i+1));
    pedals[i].autoSensing = (a == checked) ? PED_ENABLE : PED_DISABLE;

    a = httpServer.arg(String("mode") + String(i+1));
    DPRINT("[%s]\n", a.c_str());
    //pedals[i].mode = (a == "1") ? PED_ENABLE : PED_DISABLE;
    
    a = httpServer.arg(String("singlepress") + String(i+1));
    pedals[i].pressMode = (a == checked) ? PED_PRESS_1 : 0;

    a = httpServer.arg(String("doublepress") + String(i+1));
    pedals[i].pressMode += (a == checked) ? PED_PRESS_2 : 0;

    a = httpServer.arg(String("longpress") + String(i+1));
    pedals[i].pressMode += (a == checked) ? PED_PRESS_L : 0;

    a = httpServer.arg(String("polarity") + String(i+1));
    pedals[i].invertPolarity += (a == checked) ? PED_ENABLE : PED_DISABLE;
  }
  blynk_refresh();
  alert = "Saved";
  httpServer.send(200, "text/html", get_pedals_page());
}

void http_handle_post_interfaces() {
  
  String a;
  String checked("on");
  
  //httpServer.sendHeader("Connection", "close");
  for (unsigned int i = 0; i < INTERFACES; i++) {
    a = httpServer.arg(String("in") + String(i+1));
    interfaces[i].midiIn = (a == checked) ? PED_ENABLE : PED_DISABLE;

    a = httpServer.arg(String("out") + String(i+1));
    interfaces[i].midiOut = (a == checked) ? PED_ENABLE : PED_DISABLE;

    a = httpServer.arg(String("thru") + String(i+1));
    interfaces[i].midiThru = (a == checked) ? PED_ENABLE : PED_DISABLE;

    a = httpServer.arg(String("routing") + String(i+1));
    interfaces[i].midiRouting = (a == checked) ? PED_ENABLE : PED_DISABLE;

    a = httpServer.arg(String("clock") + String(i+1));
    interfaces[i].midiClock = (a == checked) ? PED_ENABLE : PED_DISABLE;
  }
  blynk_refresh();
  alert = "Saved";
  httpServer.send(200, "text/html", get_interfaces_page());
}

void http_handle_post_options() {
  
  blynk_disconnect();
  blynk_set_token(httpServer.arg(String("blynkauthtoken")));
  blynk_connect();
  blynk_refresh();
  alert = "Saved";
  httpServer.send(200, "text/html", get_options_page());
}

#endif  // WEBCONFIG


String get_update_page() {

  String page = "";

  page += get_top_page(5);

  page += F("<p></p>");
  page += "<form method='POST' action='/update' enctype='multipart/form-data'><input type='file' name='update'><input type='submit' value='Update'></form>";

  page += get_footer_page();

  return page;
}

 // handler for the /update form page

void http_handle_update () {
  if (httpServer.hasArg("theme")) theme = httpServer.arg("theme");
  // The connection will be closed after completion of the response.
  // The connection SHOULD NOT be considered `persistent'.
  // Applications that do not support persistent connections MUST include the "close" connection option in every message.
  //httpServer.sendHeader("Connection", "close");
  httpServer.send(200, "text/html", get_update_page());
}

// handler for the /update form POST (once file upload finishes)

void http_handle_update_file_upload_finish () {
  //httpServer.sendHeader("Connection", "close");
  httpServer.send(200, "text/plain", (Update.hasError()) ? "Update fail!" : "<META http-equiv='refresh' content='15;URL=/'>Update Success! Rebooting...\n");
  ESP.restart();
}

// handler for the file upload, get's the sketch bytes, and writes
// them through the Update object

void http_handle_update_file_upload() {
  
  StreamString  str;
  HTTPUpload&   upload = httpServer.upload();

  switch (upload.status) {

    case UPLOAD_FILE_START:
      {
      // Disconnect, not to interfere with OTA process
      blynk_disconnect();

      display.clear();
      display.setFont(ArialMT_Plain_10);
      display.setTextAlignment(TEXT_ALIGN_CENTER_BOTH);
      display.drawString(display.getWidth() / 2, display.getHeight() / 2 - 11, "HTTP Update");
      display.display();
      DPRINT("Filename: %s\n", upload.filename.c_str());
#ifdef ARDUINO_ARCH_ESP8266
      //size of max sketch rounded to a sector
      uint32_t maxSketchSpace = (ESP.getFreeSketchSpace() - 0x1000) & 0xFFFFF000;
      if (Update.begin(maxSketchSpace)) { //start with max available size
#endif
#ifdef ARDUINO_ARCH_ESP32
      if (Update.begin()) {  //start with max available size
#endif
        display.drawProgressBar(4, 32, 120, 8, 0);
        display.display();
        DPRINT("Update start\n");
      }
      else {
        Update.printError(str);
        DPRINT("Update start fail: %s", str.c_str());
      }
      break;
      }
    case UPLOAD_FILE_WRITE:
      if (Update.write(upload.buf, upload.currentSize) == upload.currentSize) {
        if (Update.size()) {
          display.drawProgressBar(4, 32, 120, 8, 100 * Update.progress() / Update.size());
          display.display();
          DPRINT("Progress: %5.1f%%\n", 100.0 * Update.progress() / Update.size());
        }
      }
      else {
        Update.printError(str);
        DPRINT("Update fail: %s", str.c_str());
      }
      break;

    case UPLOAD_FILE_END:
      display.clear();
      display.setFont(ArialMT_Plain_10);
      display.setTextAlignment(TEXT_ALIGN_CENTER_BOTH);
      display.drawString(display.getWidth() / 2, display.getHeight() / 2, "Restart");
      display.display();
      if (Update.end(true)) { //true to set the size to the current progress
        DPRINT("Update Success: %u\nRebooting...\n", upload.totalSize);
      } else {
        Update.printError(str);
        DPRINT("Update fail: %s", str.c_str());
      }
      break;

    case UPLOAD_FILE_ABORTED:
      display.clear();
      display.setFont(ArialMT_Plain_10);
      display.setTextAlignment(TEXT_ALIGN_CENTER_BOTH);
      display.drawString(display.getWidth() / 2, display.getHeight() / 2, "Update aborted");
      display.display();
      ESP.restart();
      break;
  }
}

void http_handle_not_found() {

  String message = "File Not Found\n\n";

  message += "URI: ";
  message += httpServer.uri();
  message += "\nMethod: ";
  message += (httpServer.method() == HTTP_GET) ? "GET" : "POST";
  message += "\nArguments: ";
  message += httpServer.args();
  message += "\n";

  for (uint8_t i = 0; i < httpServer.args(); i++) {
    message += " " + httpServer.argName(i) + ": " + httpServer.arg(i) + "\n";
  }

  httpServer.send(404, "text/plain", message);
}

void http_setup() {

#ifdef WEBCONFIG
  httpServer.on("/", http_handle_root);
  httpServer.on("/live", http_handle_live);
  httpServer.on("/banks", http_handle_banks);
  httpServer.on("/pedals", HTTP_GET, http_handle_pedals);
  httpServer.on("/pedals", HTTP_POST, http_handle_post_pedals);
  httpServer.on("/interfaces", HTTP_GET, http_handle_interfaces);
  httpServer.on("/interfaces", HTTP_POST, http_handle_post_interfaces);
  httpServer.on("/options", HTTP_GET, http_handle_options);
  httpServer.on("/options", HTTP_POST, http_handle_post_options);
#endif
  httpServer.on("/update", HTTP_GET, http_handle_update);
  httpServer.on("/update", HTTP_POST, http_handle_update_file_upload_finish, http_handle_update_file_upload);
  httpServer.onNotFound(http_handle_not_found);
}