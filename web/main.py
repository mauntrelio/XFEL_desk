#!/usr/bin/python
# -*- coding: utf-8 -*-

from RPiHTTPServer import RPiHTTPServer, RPiHTTPRequestHandler
import serial
import os
import json

class TableHandler(RPiHTTPRequestHandler):

  # GET /
  def default_response(self):
    tpl_vars = {}
    self.render_template(tpl_vars=tpl_vars)

  # POST /up
  def up(self):
    self.send_command("U")

  # POST /down
  def down(self):
    self.send_command("D")

  # POST /1
  def zero(self):
    self.send_command("0")

  # POST /1
  def one(self):
    self.send_command("1")

  # POST /2
  def two(self):
    self.send_command("2")

  # POST /3
  def three(self):
    self.send_command("3")

  # POST /stop
  def stop(self):
    self.send_command("X")

  def send_command(self, command):
    self.server.serial.write(command)
    self.render_template()

  def render_template(self, template="home.html", tpl_vars={}):
    if self.request_xhr:
      self.content_type = "application/json"
      self.content = json.dumps(tpl_vars)
    else:
      tpl = os.path.join(self.config["TEMPLATE_FOLDER"], template)
      if os.path.isfile(tpl):
        tpl_content = open(tpl,"r").read()
        # self.content = pystache.render(tpl_content, tpl_vars)
        self.content = tpl_content
      else:
        self.give_404("Template %s missing" % template)

def main():

  # read configuration from json
  basedir = os.path.dirname(os.path.abspath(__file__))
  config_file = os.path.join(basedir,"config.json")

  # instantiate http server
  WebServer = RPiHTTPServer(config_file, TableHandler)

  # quick access to config params
  config = WebServer.server.config

  # start serial and pass to web server
  WebServer.server.serial = serial.Serial(config["SERIAL_PORT"], baudrate = 9600)

  # start the web server
  try:
    print "Server listening on http://%s:%s" % (config["SERVER_ADDRESS"],config["SERVER_PORT"])
    WebServer.serve_forever()
  except KeyboardInterrupt:
    pass
    WebServer.server.serial.write("X")
    WebServer.server.server_close()


if __name__ == '__main__':

  main()
