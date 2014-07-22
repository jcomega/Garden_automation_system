/*!
 * Domotic javascript for sparkcore library v1.0.0
 * Copyright 2014 
 * Licensed under http://www.apache.org/licenses/LICENSE-2.0
 */
       $(document).ready(function() {
        var statusTimer = null;
        var timer1, timer2, timer3, timer4, timer5, timer6, timer7, timerh1, timerh2,timer8,timer9;
        var ghumi1, ghumi2, g3, g4;
        var baseURL = "https://api.spark.io/v1/devices/";
        
        //--------------------------------------------------------------
        // START - EDITABLE USER DATA
        //
        var accessToken = "37cffcfe12acb6e15bf91dbd7a3e6b2cab003ff0";
        var coreID = "48ff70065067555045571287";
        // App Heading
        var appHeading = "Page de contrôle domotique du jardin";
        // Function 1
        var funcKey1 = "cmdpump1";
        var args1 = "";
        var label1 = "Commande pompe 1"; // button label 1 (set to "" to disable)
        // Function 2
        var funcKey2 = "readbutton";
        var args2 = "";
        var label2 = "Lire le bouton poussoir"; // button label 2 (set to "" to disable)
        // Function 3
        var funcKey3 = "cmdpump2";
        var args3 = "";
        var label3 = "Commande pompe 2"; // button label 3 (set to "" to disable)
        // Function 4
        var funcKey4 = "";
        var args4 = "";
        var label4 = ""; // button label 4 (set to "" to disable)
        // Variable 1
        var varKey1 = "pump1state";
        var var1onLabel = "Pompe 1 en marche!";
        var var1offLabel = "Pompe 1 éteinte";
        var var1onState = 1; // Set to null to allow raw value to be displayed.
        var var1offState = 0; // Set to null to allow raw value to be displayed.
        var refresh1 = 5000; // variable 1 refresh rate in milliseconds (set to 0 to disable)
        // Variable 2
        var varKey2 = "buttonstate";
        var var2onLabel = "Bouton non appuyé";
        var var2offLabel = "Bouton appuyé"   ;
        var var2onState = 1; // Set to null to allow raw value to be displayed.
        var var2offState = 0; // Set to null to allow raw value to be displayed.
        var refresh2 = 5000; // variable 2 refresh rate in milliseconds (set to 0 to disable)
        // Variable 3
        var varKey3 = "pump2state";
        var var3onLabel = "Pompe 2 en marche!";
        var var3offLabel = "Pompe 2 éteinte";
        var var3onState = 1; // Set to null to allow raw value to be displayed.
        var var3offState = 0; // Set to null to allow raw value to be displayed.
        var refresh3 = 5000; // variable 3 refresh rate in milliseconds (set to 0 to disable)
        // Variable 4 (JustGauge Example)
        var varKey4 = "lightvalue";
        var var4onLabel = " "; // If using gauge, label for gauge is onLabel
        var var4offLabel = "";
        var var4onState = null; // Set to null to allow raw value to be displayed.
        var var4offState = null; // Set to null to allow raw value to be displayed.
        var refresh4 = 5000; // variable 4 refresh rate in milliseconds (set to 0 to disable)
        // Variable 5
        var varKey5 = "tempvaluew";
        var var5onLabel = ""; // If using gauge, label for gauge is onLabel
        var var5offLabel = "";
        var var5onState = null; // Set to null to allow raw value to be displayed.
        var var5offState = null; // Set to null to allow raw value to be displayed.
        var refresh5 = 5000; // variable 4 refresh rate in milliseconds (set to 0 to disable)
		// Variable 6
        var varKey6 = "temperature";
        var var6onLabel = ""; // If using gauge, label for gauge is onLabel
        var var6offLabel = "";
        var var6onState = null; // Set to null to allow raw value to be displayed.
        var var6offState = null; // Set to null to allow raw value to be displayed.
        var refresh6 = 5000; // variable 6 refresh rate in milliseconds (set to 0 to disable)
		// Variable 7
        var varKey7 = "humidity";
        var var7onLabel = ""; // If using gauge, label for gauge is onLabel
        var var7offLabel = "";
        var var7onState = null; // Set to null to allow raw value to be displayed.
        var var7offState = null; // Set to null to allow raw value to be displayed.
        var refresh7 = 5000; // variable 7 refresh rate in milliseconds (set to 0 to disable)
		
        // Variable H1 (humidité du sol 1)
        var varKeyH1 = "humidsol1";
        var varH1onLabel = " "; // If using gauge, label for gauge is onLabel
        var varH1offLabel = "";
        var varH1onState = null; // Set to null to allow raw value to be displayed.
        var varH1offState = null; // Set to null to allow raw value to be displayed.
        var refreshH1 = 5000; // variable 4 refresh rate in milliseconds (set to 0 to disable)
		
        // Variable H2 (humidité du sol 2)
        var varKeyH2 = "humidsol2";
        var varH2onLabel = " "; // If using gauge, label for gauge is onLabel
        var varH2offLabel = "";
        var varH2onState = null; // Set to null to allow raw value to be displayed.
        var varH2offState = null; // Set to null to allow raw value to be displayed.
        var refreshH2 = 5000; // variable 4 refresh rate in milliseconds (set to 0 to disable)

		// Variable 8
        var varKey8 = "waterlevel";
        var var8onLabel = ""; // If using gauge, label for gauge is onLabel
        var var8offLabel = "";
        var var8onState = null; // Set to null to allow raw value to be displayed.
        var var8offState = null; // Set to null to allow raw value to be displayed.
        var refresh8 = 5000; // variable 8 refresh rate in milliseconds (set to 0 to disable)
		
				// Variable 9
        var varKey9 = "batteryvol";
        var var9onLabel = ""; // If using gauge, label for gauge is onLabel
        var var9offLabel = "";
        var var9onState = null; // Set to null to allow raw value to be displayed.
        var var9offState = null; // Set to null to allow raw value to be displayed.
        var refresh9 = 5000; // variable 9 refresh rate in milliseconds (set to 0 to disable)
        //
        // END - EDITABLE USER DATA
        //--------------------------------------------------------------

        // Update app heading
        $("#app-heading").html(appHeading);

        // Override button styles
        //$(".btn-lg").css({"width":"55%","height":"60px","float":"left","margin-bottom":"5px"});

        // Update button labels to code definitions
        // or hide them if not defined.
        (label1) ? $("#button-1").html(label1) : $("#button-1").hide();
        (label2) ? $("#button-2").html(label2) : $("#button-2").hide();
        (label3) ? $("#button-3").html(label3) : $("#button-3").hide();
        (label4) ? $("#button-4").html(label4) : $("#button-4").hide();

        // Override text input styles
        //$(".form-control").css({"display":"inline","width":"40%","float":"right","height":"60px","margin-bottom":"5px"});

        // Hide variable text fields if not in use.
        if(refresh1 === 0) $("#var-val-1").hide();
        if(refresh2 === 0) $("#var-val-2").hide();
        if(refresh3 === 0) $("#var-val-3").hide();
        if(refresh4 === 0) $("#var-val-4").hide();
		if(refresh5 === 0) $("#var-val-5").hide();
		if(refresh6 === 0) $("#var-val-6").hide();
		if(refresh7 === 0) $("#var-val-7").hide();
        if(refreshH1 === 0) $("#var-val-H1").hide();
		if(refreshH2 === 0) $("#var-val-H2").hide();
		if(refresh8 === 0) $("#var-val-8").hide();
		if(refresh9 === 0) $("#var-val-9").hide();
		
        // Auto-refresh
        // Turn on/off the variable refresh if refresh rates are defined
        if(refresh1) {
          $("#get-var-1").attr("disabled", "disabled");
          timer1 = setInterval(function () {
            getVariable1();
          }, refresh1);
        }
        if(refresh2) {
          $("#get-var-2").attr("disabled", "disabled");
          timer2 = setInterval(function () {
            getVariable2();
          }, refresh2);
        }
        if(refresh3) {
          $("#get-var-3").attr("disabled", "disabled");
          timer3 = setInterval(function () {
            getVariable3();
          }, refresh3);
        }
        if(refresh4) {
          $("#get-var-4").attr("disabled", "disabled");
          timer4 = setInterval(function () {
            getVariable4();
          }, refresh4);
        } 
	
        if(refresh5) {
          $("#get-var-5").attr("disabled", "disabled");
          timer5 = setInterval(function () {
            getVariable5();
          }, refresh5);
        }   
        if(refresh6) {
          $("#get-var-6").attr("disabled", "disabled");
          timer6 = setInterval(function () {
            getVariable6();
          }, refresh6);
        }
        if(refresh7) {
          $("#get-var-7").attr("disabled", "disabled");
          timer7 = setInterval(function () {
            getVariable7();
          }, refresh7);
        }
        if(refreshH1) {
          $("#get-var-h1").attr("disabled", "disabled");
          timerh1 = setInterval(function () {
            getVariableH1();
          }, refreshH1);
        } 
        if(refreshH2) {
          $("#get-var-h2").attr("disabled", "disabled");
          timerh2 = setInterval(function () {
            getVariableH2();
          }, refreshH2);
        }
        if(refresh8) {
          $("#get-var-8").attr("disabled", "disabled");
          timer8 = setInterval(function () {
            getVariable8();
          }, refresh8);
        }
        if(refresh9) {
          $("#get-var-9").attr("disabled", "disabled");
          timer9 = setInterval(function () {
            getVariable9();
          }, refresh9);
        }
        ////
        // Alerts
        ////
        $("#info-alert").alert();
        $("#info-alert").affix();

        ////
        // Methods
        ////
        function onMethodSuccess() {
          alert = $("#info-alert");
          alert.text("Success!").removeClass("alert-danger").addClass("alert-success");
          if(statusTimer != null) {
            clearTimeout(statusTimer);
            alert.hide();
            setTimeout(function() {
              alert.show();
              statusTimer = setTimeout(function() {
                statusTimer = null;
                alert.hide();
              }, 1750);
            }, 250);
          }
          else {
            alert.show();
            statusTimer = setTimeout(function() {
              statusTimer = null;
              alert.hide();
            }, 2000);
          }
        }

        function onMethodFailure(data) {
          alert = $("#info-alert");
          alert.text((data)?"Error! "+data:"Error!").removeClass("alert-success").addClass("alert-danger");
          if(statusTimer != null) {
            clearTimeout(statusTimer);
            alert.hide();
            setTimeout(function() {
              alert.show();
              statusTimer = setTimeout(function() {
                statusTimer = null;
                alert.hide();
              }, 1750);
            }, 250);
          }
          else {
            alert.show();
            statusTimer = setTimeout(function() {
              statusTimer = null;
              alert.hide();
            }, 2000);
          }
        }

        // The base level run method command
        function doMethod(method, data) {
          var url = baseURL + coreID + "/" + method;
          $.ajax({
            type: "POST",
            url: url,
            data: {
              access_token: accessToken,
              args: data
            },
            dataType: "json"
          }).success(function(obj) {
            console.log(obj);
            (obj.return_value && obj.return_value == 200) ? onMethodSuccess() : onMethodFailure((obj.error)?obj.error:"");
          }).fail(function(obj) {
            onMethodFailure();
          });
        }

        $("#button-1").on("click", function () {
          doMethod(funcKey1, args1);
        });
        $("#button-2").on("click", function () {
          doMethod(funcKey2, args2);
        });
        $("#button-3").on("click", function () {
          doMethod(funcKey3, args3);
        });
        $("#button-4").on("click", function () {
          doMethod(funcKey4, args4);
        });

        ////
        // Variables
        ////
        function getVariable(variable, callback) {
          var url = baseURL + coreID + "/" + variable + "?access_token=" + accessToken;
          $.ajax({
            url: url,
            dataType: "json"
          }).success(function(obj) {
            console.log(obj);
            (obj.coreInfo.deviceID && obj.coreInfo.deviceID == coreID) ? onMethodSuccess() : onMethodFailure((obj.error)?obj.error:"");
            callback(obj.result);
          }).fail(function(obj) {
            onMethodFailure();
          });
        }

        // Get variable methods
        function getVariable1() {
          getVariable(varKey1, function (res) {
            if(res === var1onState)
              $("#var-val-1").val(var1onLabel);
            else if(res === var1offState)
              $("#var-val-1").val(var1offLabel);
            else
              $("#var-val-1").val(res);
          });
        }
        function getVariable2() {
          getVariable(varKey2, function (res) {
            if(res === var2onState)
              $("#var-val-2").val(var2onLabel);
            else if(res === var2offState)
              $("#var-val-2").val(var2offLabel);
            else
              $("#var-val-2").val(res);
          });
        }
        function getVariable3() {
          getVariable(varKey3, function (res) {
            if(res === var3onState)
              $("#var-val-3").val(var3onLabel);
            else if(res === var3offState)
              $("#var-val-3").val(var3offLabel);
            else
              $("#var-val-3").val(res);
          });
        }
        function getVariable4() {
          getVariable(varKey4, function (res) {
            g4.refresh(res); // set gauge 4 value to result from API request
          });
        }

        function getVariable5() {
          getVariable(varKey5, function (res) {
            if(res === var5onState)
              $("#var-val-5").val(var5onLabel);
            else if(res === var5offState)
              $("#var-val-5").val(var5offLabel);
            else
              $("#var-val-5").val(res);
          });
        }
        function getVariable6() {
          getVariable(varKey6, function (res) {
            if(res === var6onState)
              $("#var-val-6").val(var6onLabel);
            else if(res === var6offState)
              $("#var-val-6").val(var6offLabel);
            else
              $("#var-val-6").val(res);
          });
        }
        function getVariable7() {
          getVariable(varKey7, function (res) {
            if(res === var7onState)
              $("#var-val-7").val(var7onLabel);
            else if(res === var7offState)
              $("#var-val-7").val(var7offLabel);
            else
              $("#var-val-7").val(res);
          });
        }
        function getVariableH1() {
          getVariable(varKeyH1, function (res) {
            ghumi1.refresh(res); // set gauge  value to result from API request
          });
        }
        function getVariableH2() {
          getVariable(varKeyH2, function (res) {
            ghumi2.refresh(res); // set gauge  value to result from API request
          });
        }
        function getVariable8() {
          getVariable(varKey8, function (res) {
            if(res === var8onState)
              $("#var-val-8").val(var8onLabel);
            else if(res === var8offState)
              $("#var-val-8").val(var8offLabel);
            else
              $("#var-val-8").val(res);
          });
        }
        function getVariable9() {
          getVariable(varKey9, function (res) {
            if(res === var9onState)
              $("#var-val-9").val(var9onLabel);
            else if(res === var9offState)
              $("#var-val-9").val(var9offLabel);
            else
              $("#var-val-9").val(res);
          });
        }	
        var g4 = new JustGage({
          id: "g4", 
          value: 0, 
          min: 0,
          max: 4095,
          title: var4onLabel,
          label: "",
          gaugeWidthScale: 1.0, // How thick the arc is (0.5 is half as thick)
          showMinMax: true,
          gaugeColor: "#eee", // background gauge color
          levelColors: [ // Define array of colors to sweep through. levelColorsGradient = true
          "#ff0000",     // blue, yellow, orange, red.
          "#ff6000",
          "#ffff00",
          "#00ff00"      // 
          ],              // Optionally just define one value for solid color gauge.
          levelColorsGradient: false // Sweep through colors, or abruptly shift
        });
		
        var ghumi1 = new JustGage({
          id: "ghumi1", 
          value: 0, 
          min: 0,
          max: 3500,
          title: varH1onLabel,
          label: "",
          gaugeWidthScale: 1.0, // How thick the arc is (0.5 is half as thick)
          showMinMax: true,
          gaugeColor: "#eee", // background gauge color
          levelColors: [ // Define array of colors to sweep through. levelColorsGradient = true
          "#ff0000",     // blue, yellow, orange, red.
          "#ff6000",
          "#ffff00",
          "#00ff00"      // 
          ],              // Optionally just define one value for solid color gauge.
          levelColorsGradient: false // Sweep through colors, or abruptly shift
        });
		
        var ghumi2 = new JustGage({
          id: "ghumi2", 
          value: 0, 
          min: 0,
          max: 3500,
          title: varH2onLabel,
          label: "",
          gaugeWidthScale: 1.0, // How thick the arc is (0.5 is half as thick)
          showMinMax: true,
          gaugeColor: "#eee", // background gauge color
          levelColors: [ // Define array of colors to sweep through. levelColorsGradient = true
          "#ff0000",     // blue, yellow, orange, red.
          "#ff6000",
          "#ffff00",
          "#00ff00"      // 
          ],              // Optionally just define one value for solid color gauge.
          levelColorsGradient: false // Sweep through colors, or abruptly shift
        });
		
      });