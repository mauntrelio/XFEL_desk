var XTable = (function($,window,document,undefined) {

  "use strict";

  var allowEvent = true;

  var bind_buttons = function () {

    $(".table-control").on("mousedown touchstart",function(event){

      if (event.repeat != undefined) {
        allowEvent = !event.repeat;
      }
      if (!allowEvent) return;

      allowEvent = false;

      var command = $(this).data("command");
      $.post("/"+command);
    });

    $(".table-control").on("mouseup touchend",function(){
      $.post("/stop");
      allowEvent = true;
    });

  };

  var bind_keys = function() {
    var mapping = {};

    $(".table-control").each(function(){
      var $this = $(this);
      if ($this.data("keys")) {
        var keys = $this.data("keys").split(",");
        var command = $this.data("command");
        keys.forEach(function(key){
          mapping[key] = command;
        });
      }
    });

    document.addEventListener("keydown", function(event) {
      var command = (mapping[event.code]) ? mapping[event.code] : mapping[event.key];
      console.log(command);
      if (command) {
        if (event.repeat != undefined) {
          allowEvent = !event.repeat;
        }
        if (!allowEvent) return;
        allowEvent = false;
        $.post("/"+command);
      }
	  });

    document.addEventListener("keyup", function(event) {
      var command = (mapping[event.code]) ? mapping[event.code] : mapping[event.key];
      console.log(command);
      if (command === "up" || command === "down") {
        $.post("/stop");
        allowEvent = true;
      }
    });

  }

  var init = function() {
    bind_buttons();
    bind_keys();
  };

  return {
    init: init
  };

}(jQuery, window, document));
