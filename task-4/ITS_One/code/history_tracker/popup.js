$("#submit").click(function () {
    track();
});

$(document).keypress(function(eve){
    if(eve.which===13)
        track();
});

function track() {

    var t = Number($("#duration").val());
    var site = $("#page_url").val();
    var check_btn = $('input[type="checkbox"]');
    var start = (new Date).getTime() - (1000 * 60 * 60 * t);
    var count = 0;

    if (site == '')
        alert("Please enter url First!!!");

    else {
        if (site.slice(0, 7) != 'http://' && site.slice(0, 8) != 'https://')
            site = "https://" + site;
        
        if (check_btn.prop("checked") == true) {
            chrome.history.getVisits({ "url": site }, function (matched_sites) {
                if (matched_sites == [])
                    count = 0;
                else {
                    matched_sites.forEach(function (visted_site) {
                        if (visted_site.visitTime - start >= 0)
                            count++;
                    });
                }
                $("#output").html("<h3>Number of Vists= " + count + "</h3>");
            });

        }
        else if (check_btn.prop("checked") == false) {
            $("#output").html("<h3>Number of Vists= " + count + "</h3>");
            chrome.history.search({
                'text': site,
                'startTime': start,
                'maxResults': 1000000
            }, function (matched_items) {
                matched_items.forEach(page => {
                    if (page.url == site || site == (page.url).slice(0, site.length)) {
                        chrome.history.getVisits({ "url": page.url }, function (matched_sites) {
                            if (matched_sites == [])
                                count = 0;
                            else {
                                matched_sites.forEach(function (visted_site) {
                                    if (visted_site.visitTime - start >= 0)
                                        count++;
                                });
                            }

                            $("#output").html("<h3>Number of Vists= " + count + "</h3>");
                        });
                    }
                });
            });
        }
    }
}