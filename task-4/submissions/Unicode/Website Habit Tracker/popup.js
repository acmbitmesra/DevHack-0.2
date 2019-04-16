function hello() {
    var link = prompt("Please enter the desired url:");
    chrome.history.getVisits({url: link}, function(visitItems){
        alert("The number of times you visited "+link+" : "+visitItems.length);
    });
  } 
  document.getElementById('clickme').addEventListener('click', hello);


