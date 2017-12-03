	<!-- Turn Off Modal -->
	<div class="modal fade" id="turnOffModal" tabindex="-1" role="dialog" aria-hidden="true" aria-labelledby="turnOffModalLabel">
		<div class="modal-dialog" role="document">
			<div class="modal-content">
				<div class="modal-header">
					<h5 class="modal-title" id="turnOffModalLabel">Turn Off Heater</h5>
					<button type="button" class="close" data-dismiss="modal" aria-label="Close">
						<span aria-hidden="true">&times;</span>
					</button>
				</div>
				<div class="modal-body">
					<form action="/" method="post" id="turnOfForm">
						<input type="hidden" name="type" value="turnOff" />
						<div class="form-group">
							<label for="inputPassword" class="col-sm-2 col-form-label">Password</label>
							<div class="col-sm-10">
								<input type="password" class="form-control" id="inputPassword" placeholder="Password" name="pass">
							</div>
						</div>
					</form>
				</div>
				<div class="modal-footer">
					<button type="button" class="btn btn-secondary" data-dismiss="modal">Close</button>
					<button type="button" class="btn btn-primary" onclick="document.getElementById('turnOfForm').submit();">Turn Off</button>
				</div>
			</div>
		</div>
	</div>