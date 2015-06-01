var websocket;
var wsUri = "ws://104.236.51.159:8080/CPUMonitor/input";

try {
    websocket = new WebSocket(wsUri);
} catch (erro) {
    $('#idDiv').html('<span style="color: red;">ERRO NA CONEXÃO:</span> ' + erro);

}
websocket.binaryType = "arraybuffer";

websocket.onmessage = function (evt) {
    if (typeof evt.data === "string") {
        var cpuJson = JSON.parse(evt.data);        
        $('#idDiv2').html('<span style="color: red;">'+cpuJson["cpuUsage"]+cpuJson["memoryUsage"]+cpuJson["date"]+'</span> ');
        
        graficoCpu(cpuJson["cpuUsage"], cpuJson["memoryUsage"], cpuJson["date"], cpuJson["hostname"], false);
    } else { }
};
websocket.onerror = function (evt) {};

function graficoCpu(cpuUsage, memoryUsage, date, hostname, inicial){  
    
    // se não existir a div -> cria; se existir -> pular para o getElement
    var div= document.getElementById(hostname);
    if (!div) {
        div = document.createElement("div");
        div.setAttribute("id", hostname);
        div.style.float = "left";
        div.style.padding = "5px";
        var date = date;
        div.innerHTML = "<div id='title"+hostname+"' align='center'>"+hostname+" - "+date+"</div><div id='canvas' align='center'><canvas id='cpuUsage"+hostname+"' width='100' height='150' align='center'></canvas><canvas id='memoryUsage"+hostname+"' width='100' height='150' align='center'></canvas></div>";
        document.body.appendChild(div);
    }
    
    var divTitle= document.getElementById("title"+hostname);
    divTitle.innerHTML =hostname+" - "+date;
    var canvas1 = document.getElementById('cpuUsage'+hostname);
    var grafico = canvas1.getContext('2d');
    
    canvas1.width = canvas1.width;

//    for (var x = 0.5; x < 110; x += 10) {
//        grafico.moveTo(x, 0);
//        grafico.lineTo(x, 160);
//    }
//    for (var y = 0.5; y < 160; y += 10) {
//        grafico.moveTo(0, y);
//        grafico.lineTo(110, y);
//    }
    grafico.strokeStyle = "#eee";
    grafico.stroke();
    
    grafico.save();    
    grafico.beginPath();    
    grafico.font="10px Arial";
    grafico.fillText("CPU Usage", 25, 20);
    grafico.fillText(cpuUsage+"%", 40, 30); 
    grafico.restore();

    grafico.beginPath();
    grafico.strokeStyle = 'black';
    
    for (var altura = 40; altura < 150; altura+=10) {
        grafico.moveTo(20, altura);
        grafico.lineTo(80, altura);
    }
//    grafico.moveTo(20, 40);                
//    grafico.lineTo(20, 140);
//    grafico.moveTo(80, 40);                
//    grafico.lineTo(80, 140);

    grafico.stroke();
    
    grafico.beginPath();
    grafico.restore();
    grafico.fillStyle = 'red';
    grafico.fillRect(20,140-cpuUsage,60,cpuUsage);
    grafico.stroke();

    grafico.beginPath();
    grafico.restore();
    grafico.strokeStyle = 'black';
    grafico.lineWidth="3";
    grafico.rect(0,0,110,150);
    grafico.stroke();
    
    grafico.beginPath();
    grafico.restore();
    grafico.strokeStyle = 'black';
    grafico.lineWidth="3";
    grafico.rect(20,40,60,100);
    grafico.stroke();

    grafico.save();
    
    var canvas2 = document.getElementById('memoryUsage'+hostname);
    var grafico = canvas2.getContext('2d');
    
    canvas2.width = canvas2.width;

//    for (var x = 0.5; x < 110; x += 10) {
//        grafico.moveTo(x, 0);
//        grafico.lineTo(x, 160);
//    }
//    for (var y = 0.5; y < 160; y += 10) {
//        grafico.moveTo(0, y);
//        grafico.lineTo(110, y);
//    }
//    grafico.strokeStyle = "#eee";
//    grafico.stroke();
    
    grafico.save();    
    grafico.beginPath();    
    grafico.font="10px Arial";
    grafico.fillText("Memory Usage", 20, 20);    
    grafico.fillText(memoryUsage+"%", 40, 30); 
    grafico.restore();

    grafico.beginPath();
    grafico.strokeStyle = 'black';
    
    for (var altura = 40; altura < 150; altura+=10) {
        grafico.moveTo(20, altura);
        grafico.lineTo(80, altura);
    }
//    grafico.moveTo(20, 40);                
//    grafico.lineTo(20, 140);
//    grafico.moveTo(80, 40);                
//    grafico.lineTo(80, 140);

    grafico.stroke();
    
    grafico.beginPath();
    grafico.restore();
    grafico.fillStyle = 'red';
    grafico.fillRect(20,140-memoryUsage,60,memoryUsage);
    grafico.stroke();
    
    grafico.beginPath();
    grafico.restore();
    grafico.strokeStyle = 'black';
    grafico.lineWidth="3";
    grafico.rect(20,40,60,100);
    grafico.stroke();

    grafico.save();
    
    grafico.beginPath();
    grafico.restore();
    grafico.strokeStyle = 'black';
    grafico.lineWidth="3";
    grafico.rect(0,0,100,150);
    grafico.stroke();
}