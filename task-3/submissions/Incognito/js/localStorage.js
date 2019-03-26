for(var key in localStorage){
    if(key=="length" || key=="key" || key=="getItem")
        break;
    document.write("<option value='" +key+ "'>" +key+ "</option>");   
}