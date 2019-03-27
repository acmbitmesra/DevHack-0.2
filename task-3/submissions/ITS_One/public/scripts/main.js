var oDoc;
var align_btn=$(".toolbar a.align");
initDoc();
function initDoc() {
    oDoc = $("#editor-textbox"); 
    init_btn_listener();

 }
 function init_btn_listener(){
  
  align_btn.click(function(){
    
      align_btn.removeClass("selected");
      $(this).addClass("selected");
});
}

  function formatDoc(sCmd, sValue) {
     oDoc.focus();
     document.execCommand(sCmd, false, sValue); 
     if(sCmd=='fontsize' && $( "#bg-color option:selected" ).text()!=='white')
     {
       var color=$( "#bg-color option:selected" ).text();
       //console.log($( "#bg-color option:selected" ).text());
      document.execCommand('backcolor',false,'white');
      document.execCommand('backcolor',false,color);
     }
    }

function add_image() {
  oDoc.focus();
  var url =document.getElementById("image-url").value;
  var width=document.getElementById("image-url").value;
  var height=document.getElementById("image-height").value;
  document.execCommand("insertImage", false, url); 
  oDoc.focus(); 
  var image=$("img");
  image.css("display", "block");
  image.css("margin","auto");
  image.css("width",width+"px");
  image.css("height",height+"px");
}

function add_url(){
  oDoc.focus();
  var url =document.getElementById("urls-url").value;
  var s ="<a href="+url+">";
  /*if($("#blank_check").value=="on")
    s+="target=\"blank\">"+$(#url-disp_text).value+"</a>";
  else*/
    s+=$("#url-disp_text").val()+"</a>";
//console.log(s);
  document.execCommand("insertHTML", false, s); 
}


function save_doc()
{
  var cont= document.getElementById("editor-textbox").innerHTML;
   cont=cont.replace(/\n/g, " ");
  var data={
    name:$("#doc-name").val(),
    author:$("#doc-owner").val(),
    content:cont,
  };
  $.post("/docs/save",data)
  .done(function(red_url){
    window.location.href=red_url;
  });
  saving();
}
function update_doc()
{
  var id=(window.location.href).slice((window.location.href).lastIndexOf('/')+1,);
  if(id[id.length-1]=='#')
    id=id.slice(0,id.length-1);
  if(id=='')
    save_doc();
  else{
    console.log(id);
  var cont= document.getElementById("editor-textbox").innerHTML;
   cont=cont.replace(/\n/g, " ");
  var data={
    name:$("#doc-name").val(),
    author:$("#doc-owner").val(),
    content:cont,
    id:id
  };
  $.post("/docs/update",data)
  .done(function(a){
    console.log("doc updated");
    $("#saved").css("display","block");
    $("#saved").addClass("show");
    setTimeout(function(){
      $("#saved").removeClass("show");
      $("#saved").css("display","none");
    },2000);
  });
}
saving();
}
var Doc_saved = false;
var saving = function() { Doc_saved = true; };
var unsaved= function() {Doc_saved=false;};
window.onload = function() {
    window.addEventListener("beforeunload", function (e) {
        if (Doc_saved) {
            return undefined;
        }

        var confirmationMessage = 'It looks like you have been editing something. '
                                + 'If you leave before saving, your changes will be lost.';

        (e || window.event).returnValue = confirmationMessage; //Gecko + IE
        return confirmationMessage; //Gecko + Webkit, Safari, Chrome etc.
    });
};
