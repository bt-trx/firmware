function setSelectedIndexByValue(s, v) {
  for (var i = 0; i < s.options.length; i++) {
    if (s.options[i].value == v) {
      s.options[i].selected = true;
      return;
    }
  }
}

function getAllParameters() {
  getDropdownData("adc_gain");
  getDropdownData("dac_gain");
  getDropdownData("ptt_mode");
  getData("ptt_hang_time");
  getData("ptt_timeout");
  getData("pin_code");
};

function getData(parameter) {
  var xhttp = new XMLHttpRequest();
  xhttp.onreadystatechange = function() {
    if (this.readyState == 4 && this.status == 200) {
      var x = document.getElementById(parameter);
      x.value = this.responseText;
    }
  };
  xhttp.open("GET", "get?id=" + parameter, true);
  xhttp.send();
}

function getCheckboxData(parameter) {
  var xhttp = new XMLHttpRequest();
  xhttp.onreadystatechange = function() {
    if (this.readyState == 4 && this.status == 200) {
      var x = document.getElementById(parameter);
      if (this.responseText == "1") {
        x.checked = true;
      } else {
        x.checked = false;
      }
    }
  };
  xhttp.open("GET", "get?id=" + parameter, true);
  xhttp.send();
}

function getDropdownData(parameter) {
  var xhttp = new XMLHttpRequest();
  xhttp.onreadystatechange = function() {
    if (this.readyState == 4 && this.status == 200) {
      setSelectedIndexByValue(document.getElementById(parameter),
                              this.responseText);
    }
  };
  xhttp.open("GET", "get?id=" + parameter, true);
  xhttp.send();
}

function setData(name, value) {
  var xhttp = new XMLHttpRequest();
  xhttp.onreadystatechange = function() {
    if (this.readyState == 4 /*&& this.status == 200*/) {
      showSnackbar(this.responseText);
    }
  };
  xhttp.open("GET", "set?id=" + name + "&value=" + value, true);
  xhttp.send();
}

function OnCheckboxChange(checkbox) {
  var xhttp = new XMLHttpRequest();
  xhttp.onreadystatechange = function() {
    if (this.readyState == 4 /*&& this.status == 200*/) {
      showSnackbar(this.responseText);
      getCheckboxData(checkbox.name);
    }
  };
  xhttp.open("GET",
              "set?id=" 
              + checkbox.name + "&value=" + checkbox.checked,
              true);
  xhttp.send();
}

function OnDropdownChange(dropdown) {
  var xhttp = new XMLHttpRequest();
  xhttp.onreadystatechange = function() {
    if (this.readyState == 4 /*&& this.status == 200*/) {
      showSnackbar(this.responseText);
      getDropdownData(dropdown.name);
    }
  };
  var index  = dropdown.selectedIndex;
  xhttp.open("GET",
              "set?id=" 
              + dropdown.name + "&value=" + dropdown.options[index].value,
              true);
  xhttp.send();
}

function OnButtonClick(actionname) {
  var xhttp = new XMLHttpRequest();
  xhttp.onreadystatechange = function() {
    if (this.readyState == 4 /*&& this.status == 200*/) {
      showSnackbar(this.responseText);
    }
  };
  xhttp.open("GET", "action?id=" + actionname, true);
  xhttp.send();
}

function showSnackbar(snackbarText) {
/* Get the snackbar DIV */
var x = document.getElementById("snackbar");

x.innerHTML = snackbarText;

/* Add the "show" class to DIV */
x.className = "show";

/* After 3 seconds, remove the show class from DIV */
setTimeout(function(){ x.className = x.className.replace("show", ""); }, 3000);
}