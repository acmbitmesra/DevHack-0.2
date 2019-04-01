if(typeof jQuery!=='undefined'){
    console.log('jQuery Loaded');
}
else{
    console.log('not loaded yet');
}

$(document).ready(function(){
    var fonts = document.querySelectorAll("select#fontChange > option");
    for(var i = 0;i < fonts.length;i++) {
        $("#font"+i).css({
            "font-family" : fonts[i].value
        }); 
    }
});
