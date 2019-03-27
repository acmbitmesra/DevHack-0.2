  var edit=false;
  $("#description-bar").css("display","none");
  $("#toolbar-1").css("display","none");
  $("#toolbar-2").css("display","none");
  $("#edit_btn").click(function(){
    if(!edit){
  $('#editor-textbox').attr('contenteditable','true');
  edit=true;
  $(this).text("View Mode");
  $("#description-bar").css("display","flex");
  $("#toolbar-1").css("display","flex");
  $("#toolbar-2").css("display","flex");
  }
  else
  {
    $('#editor-textbox').attr('contenteditable','false');
  edit=false;
  $(this).text("Edit Mode");
  $("#description-bar").css("display","none");
  $("#toolbar-1").css("display","none");
  $("#toolbar-2").css("display","none");
  }
  });
  
  $("#share_link").text(window.location.href);