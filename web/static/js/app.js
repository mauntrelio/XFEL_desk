var App = (function($,window,document,undefined){

  "use strict";

  var App = {};

  App.init = function(){
    // init GUI interaction
    if (typeof XTable !== "undefined") {
      XTable.init();
    }
  }

  return App;

}(jQuery,window,document));


$(function(){
  App.init();
})
