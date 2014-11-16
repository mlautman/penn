# Kalman filtering of raw data
from pykalman import KalmanFilter

def kalman(raw_data):
	# kf = KalmanFilter(initial_state_mean = 0, n_dim_obs = 2)
	kf = KalmanFilter(transition_matrices = [[1, 1], [0, 1]], observation_matrices = [[0.1, 0.5], [-0.3, 0.0]])
	measurements = np.asarray([[1,0], [0,0], [0,1]])  # 3 observations
	kf = kf.em(measurements, n_iter=5)
	(filtered_state_means, filtered_state_covariances) = kf.filter(measurements)
	(smoothed_state_means, smoothed_state_covariances) = kf.smooth(measurements)