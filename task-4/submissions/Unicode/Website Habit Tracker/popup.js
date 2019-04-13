function hello() {
    var url = prompt("Please enter the desired url:");
    chrome.history.getVisits({url: url}, function(visitItems){
        alert("The number of times you visited a website:"+visitItems.length);
    });
  } 
  document.getElementById('clickme').addEventListener('click', hello);