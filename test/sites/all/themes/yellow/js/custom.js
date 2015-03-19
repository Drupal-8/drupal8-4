//===================================================================== ok ==
function getfullscreen() {
    var winWidth2 = jQuery(window).width();
    if (winWidth2 <= 1024 && winWidth2 > 767) {
        jQuery("#block-views-banner-block img, .view-inner-banner img").each(function() {

            var image = new Image();
            image.src = jQuery(this).attr("src");
            image.onload = function() {
                var size1 = new Array();
                var Imgheight = this.height;
                var Imgheight1 = Imgheight * (80 / 100);
                var Imgwidth = this.width;
                var Imgwidth1 = Imgwidth * (80 / 100);
                size1['height'] = Imgheight1;
                size1['width'] = Imgwidth1;
                //   var marg_left =  (Imgwidth - Imgwidth1) / 2 ;
                var marg_left = (Imgwidth1 - winWidth2) / 2;
                size1['width-mar'] = marg_left;
                return size1;
            };
            jQuery(this).parent().css({'width': image.onload()['width'], 'height': image.onload()['height'], 'overflow': 'hidden'});
            jQuery(this).css({'width': image.onload()['width'], 'height': image.onload()['height'], 'margin-left': -image.onload()['width-mar']});

        })

        // ========= other images ============//bleed_top
        jQuery(".slider_images_with_description_ img, .half_image_right .bleed_top img,.half_image_right .bleed_mid img, .half_image_right .bleed_buttom img,.half_image_left .bleed_top img,.half_image_left .bleed_mid img,.half_image_left .bleed_buttom img, .Company-logo-section img,.three_images_text img, .text_with_background_image img, #block-block-1 img,#block-block-5 img,.get-right-image img,.view-client-logo img,.normal_paragraph img").each(function() {
            var image = new Image();
            image.src = jQuery(this).attr("src");
            image.onload = function() {
                var size1 = new Array();
                var Imgheight = this.height;
                var Imgheight1 = Imgheight * (80 / 100);
                var Imgwidth = this.width;
                var Imgwidth1 = Imgwidth * (80 / 100);
                size1['height'] = Imgheight1;
                size1['width'] = Imgwidth1;
                return size1;
            };
            jQuery(this).css({'width': image.onload()['width'], 'height': image.onload()['height']});

        })

    }
    else if (winWidth2 <= 767) {
        jQuery("#block-views-banner-block img, .view-inner-banner img").each(function() {
            var image = new Image();
            image.src = jQuery(this).attr("src");
            image.onload = function() {
                var size1 = new Array();
                var Imgheight = this.height;
                var Imgheight1 = Imgheight * (50 / 100);
                var Imgwidth = this.width;
                var Imgwidth1 = Imgwidth * (50 / 100);
                size1['height'] = Imgheight1;
                size1['width'] = Imgwidth1;
                //   var marg_left =  (Imgwidth - Imgwidth1) / 2 ;
                var marg_left = (Imgwidth1 - winWidth2) / 2;
                size1['width-mar'] = marg_left;
                return size1;
            };
            jQuery(this).parent().css({'width': image.onload()['width'], 'height': image.onload()['height'], 'overflow': 'hidden'});
            jQuery(this).css({'width': image.onload()['width'], 'height': image.onload()['height'], 'margin-left': -image.onload()['width-mar']});

        })
        // ========= other images ============//
        jQuery(".slider_images_with_description_ img, .half_image_right .bleed_top img,.half_image_right .bleed_mid img, .half_image_right .bleed_buttom img,.half_image_left .bleed_top img,.half_image_left .bleed_mid img,.half_image_left .bleed_buttom img, .Company-logo-section img,.three_images_text img, .text_with_background_image img, #block-block-1 img,#block-block-5 img,.get-right-image img,.view-client-logo img,.normal_paragraph img").each(function() {
            var image = new Image();
            image.src = jQuery(this).attr("src");
            image.onload = function() {
                var size1 = new Array();
                var Imgheight = this.height;
                var Imgheight1 = Imgheight * (50 / 100);
                var Imgwidth = this.width;
                var Imgwidth1 = Imgwidth * (50 / 100);
                size1['height'] = Imgheight1;
                size1['width'] = Imgwidth1;
                return size1;

            };
            jQuery(this).css({'width': image.onload()['width'], 'height': image.onload()['height']});

        })
    }
    else {
        jQuery(".half_image_right .bleed_top img,.half_image_right .bleed_mid img, .half_image_right .bleed_buttom img,.half_image_left .bleed_top img,.half_image_left .bleed_mid img,.half_image_left .bleed_buttom img, .view-inner-banner img, #block-views-banner-block img, .slider_images_with_description_ img, .Company-logo-section img, .three_images_text img, .text_with_background_image img, #block-block-1 img ,#block-block-5 img,.get-right-image img ,.view-client-logo img, .normal_paragraph img").each(function() {
            var image = new Image();
            image.src = jQuery(this).attr("src");
            image.onload = function() {
                var size1 = new Array();
                var Imgheight = this.height;
                var Imgwidth = this.width;

                size1['height'] = Imgheight;
                size1['width'] = Imgwidth;
                return size1;
            };
            jQuery(this).css({'width': image.onload()['width'], 'height': image.onload()['height']});

        })
    }

}

function menu_resize() {
    var winWidth = jQuery(window).width();
    // For Menu Scroll ================================================== ok//
    if (winWidth >= 970) {
        jQuery("#superfish-2 .phone").remove();
        jQuery("#superfish-2").append("<li class='phone'><span class='phone-tab'>085 40 179 52</span></li>");
        if (jQuery('body').hasClass('front')) {
            jQuery(window).scroll(function() {

                if (jQuery(this).scrollTop() >= 800) {
                    jQuery('#upper-nav').removeClass('hide1');
                    jQuery("#superfish-1 li.phone").show();
                }
                else {
                    jQuery('#upper-nav').addClass('hide1');
                    jQuery("#superfish-1 li.phone").hide();
                }
            });
        } else {

            jQuery(window).scroll(function() {
                if (jQuery(this).scrollTop() >= 300) {
                    jQuery('#upper-nav').removeClass('hide1');
                    jQuery("#superfish-1 li.phone").show();

                }
                else {
                    jQuery('#upper-nav').addClass('hide1');
                    jQuery("#superfish-1 li.phone").hide();
                }
            });
        }
    }
    else {
        jQuery('#upper-nav').addClass('hide1');
    }

}

function footerchanges() {
    var winWidth = jQuery(window).width();

// ============== Footer changes for responsive ======================= ok//
    if (winWidth <= 768) {

        jQuery("#block-block-3 .newsletter").clone().appendTo("#block-block-2");
        jQuery("#block-block-3 .newsletter").hide();
        jQuery("#block-block-3 .followus-footer").clone().appendTo("#block-block-4");
        jQuery("#block-block-3 .followus-footer").hide();
    } else {
        jQuery("#block-block-2 .newsletter").hide();
        jQuery("#block-block-4 .followus-footer").hide();
        jQuery("#block-block-3 .newsletter").show();
        jQuery("#block-block-3 .followus-footer").show();
    }
}

jQuery(document).ready(function($) {

    var winWidth = $(window).width();

// ============== Footer changes for responsive ======================= ok//
    footerchanges();

    //======================= Sidebar in 480 section =================//

    if ((winWidth < 480)) {
        $("#block-views-blog-terms-block .view-header,#block-views-blogs-block-1 .view-header").addClass("side-effects");
        $("#block-views-blog-terms-block .view-content").hide();
        $("#block-views-blog-terms-block .view-header").click(function() {
            $("#block-views-blog-terms-block .view-content").slideToggle("slow");
        });

        $("#block-views-blogs-block-1 .view-content").hide();
        $("#block-views-blogs-block-1 .view-header").click(function() {
            $("#block-views-blogs-block-1 .view-content").slideToggle("slow");
        });

    }

    // =================== For homepage paragraph Slider ========================== ok//
    var slider_para = $(".slider_images_with_description_").hasClass("remind");
    if (slider_para) {
        $('.flexslider').flexslider({
            animation: "slide",
            slideshow: false,
            start: function(slider) {
                $('body').removeClass('loading');
            }
        });
        $('.flexslider1').flexslider({
            animation: "slide",
            slideshow: false,
            start: function(slider) {
                $('body').removeClass('loading');
            }
        });
    }


    //=====================================================================//

    $(window).load(function() {
        getfullscreen();
    })
    menu_resize();
    bleed_images_resize();

////======================= Sidebar in 480 section =================//

    if (winWidth <= 480) {
        $("#block-views-blog-terms-block .view-header,#block-views-blogs-block-1 .view-header").addClass("side-effects");
        $("#block-views-blog-terms-block .view-content").hide();
        $("#block-views-blog-terms-block .view-header").click(function() {
            $("#block-views-blog-terms-block .view-content").toogle("slow");
        });

        $("#block-views-blogs-block-1 .view-content").hide();
        $("#block-views-blogs-block-1 .view-header").click(function() {
            $("#block-views-blogs-block-1 .view-content").toogle("slow");
        });

    }

    ///============  Logo Menu =============//

    $("#navbar .navbar-toggle").click(function() {
        $("#navbar").toggleClass("white-color");
    });
//==============================================


    if ($('#block-formalter-home-blog > h2.block-title').length) {
       // alert(1);
    $("#block-formalter-home-blog > h2.block-title").hide();
    }
});

function bleed_images_resize(){
    //=================  For Paragraph Module Image == //
var winWidth = jQuery(window).width();
    if (winWidth > 767) {

        jQuery('div.half_image_left').each(function() {
            var para_left_img = jQuery(this).find("div.left").height();
            var para_leftt_txt = jQuery(this).find("div.right").height();

            if (para_leftt_txt > para_left_img) {

                jQuery(this).find("div.left").height(para_leftt_txt);
                if (jQuery(this).find("div.left").hasClass("bleed_mid")) {
                    //   console.log();
                    jQuery(this).find("img").addClass('mid-img');
                }
                if (jQuery(this).find("div.left").hasClass("bleed_fix")) {
                    jQuery(this).find("img").addClass('mid-img');
                }
            }

        });

        jQuery("div.half_image_right").each(function() {

            var para_right_txt = jQuery(this).find("div.left").height();
            var para_right_img = jQuery(this).find("div.right").height();

            if (para_right_txt > para_right_img) {

                jQuery(this).find("div.right").height(para_right_txt);
                if (jQuery(this).find("div.right").hasClass("bleed_mid")) {
                   jQuery(this).find("img").addClass('mid-img');
                }
                if (jQuery(this).find("div.right").hasClass("bleed_fix")) {
                    jQuery(this).find("img").addClass('mid-img');
                }
            }
        });
    }
}

jQuery(window).resize(function() {
    getfullscreen();
    menu_resize();
    footerchanges();
    bleed_images_resize();
});
