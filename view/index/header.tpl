<!DOCTYPE html>
<html>
	<head>
		<title>TanTa-首页</title>
		<meta charset="utf-8"/>
		<link href="/static/css/bootstrap.min.css" rel="stylesheet">
		<meta name="viewport" content="user-scalable=no, width=device-width, initial-scale=1.0" />
		<script src="/static/js/jquery.js"></script>
		<script src="/static/js/bootstrap.min.js"></script>
		<script type="text/javascript" >
			$(document).ready(function () {
			
				var sections = $("section");
				sections.addClass("well");
				sections.click(function (d) {
					try{
						var link = d.currentTarget.children[0].firstChild.href;
						window.location.href=link;
					}catch (e) {
						
					}
				});
				
				$(".classify").click(function (d) {
					try{
						var classify = d.currentTarget.innerText;
						window.location.href="/?classify="+encodeURIComponent(classify);
					}catch (e) {
						
					}
					return false;
				});
				
				$("#top-menu").height(window.screen.height);
				
				window.OriginTitile = document.title;
				function leavingPage() {
					if (document.hidden) {
						document.title = "页面炸了！点击恢复！";
						clearTimeout(titleTime);
					} else {
						document.title = "(/≧▽≦/)咦！又好了！ " + window.OriginTitile;
						titleTime = setTimeout(function() {
								document.title = window.OriginTitile;
							},
							2000
						);
					}
				}
				//window.onfocus=leavingPage;
				document.addEventListener("visibilitychange",leavingPage);
			});
			
		</script>
		<style type="text/css">
			html,body{
				height:100%;
			}
			.fixed {
				width: 1000px;
				height: 70px;
				position: fixed;
				top: -1px;
				left: 50%;
				margin-left: -500px;
				z-index: 1000;
			}
			.psg-title{
				font-size:30px;
			}
			.psg-user{
				display: inline;
				padding-right: 1em;
				color: rgba(0,0,0,0.3);
			}
			.classify{
				display: inline;
				padding-right: 1em;
				color: rgba(0,0,0,0.3);
			}
			time{
				color: rgba(0,0,0,0.3);
			}
		</style>
	</head>
	<body style="background:url(/static/img/bkg.png) no-repeat center fixed;background-size:1800px 850px;">
		<div class="container" id="top-menu">
			<div style="top:50%;left:50%;transform:translate(-50%, -50%);color:#fff;position: absolute;font-size: 200%;text-align:center;">
				<h1 style="font-size: 200%;">
					<a href="/" style="color:#fff;">cgoxpx's blog</a>
				</h1>
				SingingRivuletProject<br>
				<span style="color: #aef;font-size: 50%;">Powered by TanTa</span>
			</div>
		</div>
		<div class="container">
		<div class="row clearfix">
			<div class="col-md-2 column">
			</div>
			<div class="col-md-8 column" style="background:rgba(255,255,255,1);border: 1px solid #e3e3e3;border-radius: 4px;">
				<br/>
				<h2>文章</h2>
