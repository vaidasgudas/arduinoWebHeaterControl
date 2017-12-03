	<!-- Turn On Modal -->
	<div class="modal fade" id="turnOnModal" tabindex="-1" role="dialog" aria-hidden="true" aria-labelledby="turnOnModalLabel">
		<div class="modal-dialog" role="document">
			<div class="modal-content">
				<div class="modal-header">
					<h5 class="modal-title" id="turnOnModalLabel">Turn On Heater</h5>
					<button type="button" class="close" data-dismiss="modal" aria-label="Close">
						<span aria-hidden="true">&times;</span>
					</button>
				</div>
				<div class="modal-body">
					<form action="/" method="post" id="turnOnForm">
						<input type="hidden" name="type" value="turnOn" />
						<div class="form-group">
							<label for="inputPassword" class="col-sm-2 col-form-label">Password</label>
							<div class="col-sm-10">
								<input type="password" class="form-control" id="inputPassword" placeholder="Password" name="pass" />
							</div>
						</div>
						<div class="form-group">
							<label for="forPeriod" class="col-sm-2 col-form-label">For</label>
							<div class="col-sm-10">
								<select class="form-control" id="forPeriod" name="for">
									<optgroup>
										<option value="t-1200">20 min</option>
										<option value="t-2400">40 min</option>
										<option value="t-3600">1 hour</option>
										<option value="t-7200">2 hour</option>
									</optgroup>
									<optgroup>
										<option value="d-20">&#916; 0.2 &#8451;</option>
										<option value="d-40">&#916; 0.4 &#8451;</option>
										<option value="d-100">&#916; 1 &#8451;</option>
									</optgroup>
								</select>
							</div>
						</div>
					</form>
				</div>
				<div class="modal-footer">
					<button type="button" class="btn btn-secondary" data-dismiss="modal">Close</button>
					<button type="button" class="btn btn-warning" onclick="document.getElementById('turnOnForm').submit();">Turn On</button>
				</div>
			</div>
		</div>
	</div>