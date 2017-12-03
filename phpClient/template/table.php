	<table class="table table-dark">
		<tbody>
			<?php if($heaterOn): ?>
				<tr class="bg-danger">
					<td>Heater state</td>
					<td>
						Heating! 
						<?php if($toas != 0): ?>
							<span id="countdown" class="badge badge-warning"><?=$toas?></span>
						<?php else: ?>
							<span class="badge badge-warning"> Till: <?=($toot/100)?><small> &#8451;</small></span>
						<?php endif; ?>
						<button type="button" class="btn btn-primary btn-sm float-right" data-toggle="modal" data-target="#turnOffModal">
							Turn Off
						</button>
					</td>
				</tr>
			<?php else: ?>
				<tr class="bg-primary">
					<td>Heater state</td>
					<td>
						Off
						<button type="button" class="btn btn-warning btn-sm float-right" data-toggle="modal" data-target="#turnOnModal">
							Turn On
						</button>
					</td>
				</tr>
			<?php endif; ?>
			<tr>
				<td>Temperature</td>
				<td><?=$temperature?><small> &#8451;</small></td>
			</tr>
			<tr>
				<td>Humidity</td>
				<td><?=$humidity?><small>%</small></td>
			</tr>
			<tr>
				<td colspan="2" class="center">
					<a href="/" class="btn btn-secondary btn-sm float-right" role="button" aria-pressed="true">Refresh</a>
				</td>
			</tr>
			<tr>
				<td colspan="2"></td>
			</tr>
		</tbody>
	</table>