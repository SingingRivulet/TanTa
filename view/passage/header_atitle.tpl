-TanTa</title>
		<meta charset="utf-8"/>
		<link href="/static/css/bootstrap.min.css" rel="stylesheet">
		<meta name="viewport" content="user-scalable=no, width=device-width, initial-scale=1.0" />
		<script src="/static/js/jquery.js"></script>
		<script src="/static/js/bootstrap.min.js"></script>
		<script src="/static/js/main.js"></script>
		<script type="text/javascript" >
			window.passage=sendPassage();
			window.psgid=window.location.href.split("/")[4];
			$(document).ready(function () {
				$("section").addClass("well");
				$("#top-menu").height(window.screen.height*0.5);
				window.OriginTitile = document.title;
				function leavingPage() {
					if (document.hidden) {
						document.title = "页面炸了！点击恢复！";
						clearTimeout(titleTime);
					} else {
						document.title = "(/≧▽≦/)咦！又好了！ " + window.OriginTitile;
						titleTime = setTimeout(function() {
								document.title = window.OriginTitile
							},
							2000
						);
					}
				}
				//window.onfocus=leavingPage;
				document.addEventListener("visibilitychange",leavingPage);
				passage.isloged(function (res) {
					if(res){
						var div = document.createElement("div");
						
						var ed  = document.createElement("span");
						ed.onclick=function () {
							window.open("/static/edit.html#"+window.psgid);
						};
						ed.innerHTML="编辑";
						ed.style.paddingRight="1em";
						
						var dl  = document.createElement("span");
						dl.onclick=function () {
							if(!confirm("确定吗"))
								return;
							passage.del(window.psgid,function () {
								window.location.href="/";
							});
						};
						dl.innerHTML="删除";
						dl.style.paddingRight="1em";
						
						div.style.float="right";
						div.appendChild(ed);
						div.appendChild(dl);
						$("#main-content").append(div);
					}
				});
			});
			
		</script>
		<style type="text/css">
			.psg-title{
				font-size:50px;
			}
			.psg-user{
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
			<div style="top:30%;left:50%;transform:translate(-50%, -50%);color:#fff;position: absolute;font-size: 200%;text-align:center;">
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
			<div class="col-md-8 column well" style="background:rgba(255,255,255,1);" id="main-content">
			<br>